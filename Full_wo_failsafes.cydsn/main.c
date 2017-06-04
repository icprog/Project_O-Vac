/*******************************************************************************
* File Name: main.c
*
* Version: 2.20
*
* Description:
*   This is a source code for example project of ADC single ended mode.
*
*   Variable resistor(pot) is connected to +ve input of ADC using the I/O pin.
*   P0.0. When voltage to positive terminal of ADC is 0, the output displayed
*   on the LCD pannel is 0x0000. As voltage on positive terminal goes on
*   increasing, the  converted value goes on increasing from 0x0000 and reaches
*   0xFFFF when voltage becomes 1.024V. Futher increase in voltage value,
*   doesn't cause any changes to values displayed in the LCD.
*
* Hardware Connections: 
*  Connect analog input from Variable resistor to port P0[0] of DVK1 board.
*
********************************************************************************
* Copyright 2012-2015, Cypress Semiconductor Corporation. All rights reserved.
* This software is owned by Cypress Semiconductor Corporation and is protected
* by and subject to worldwide patent and copyright laws and treaties.
* Therefore, you may use this software only as provided in the license agreement
* accompanying the software package from which you obtained this software.
* CYPRESS AND ITS SUPPLIERS MAKE NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* WITH REGARD TO THIS SOFTWARE, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT,
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*******************************************************************************/

#include <project.h>
#include <mpu6050.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <FS.h>
#include "LiquidCrystal_I2C.h"
#include "functions.h"

#define MPU6050 
#define LCD
#define SD
#define BT

#define MA_WINDOW 15                    // Number of samples in the moving average window.
#define BOT_THRESHOLD 20000             // Z-Aacceleration threshold for transition into LANDED state.
#define WAIT_TIME 1000                  // Number of ISR calls until transition into DESCENDING state.
#define BUFFER_LEN  64u                 // Buffer length for UART rx
#define DEGREES_20 (131 * 20)           // Gyro value corresponding to 30 degrees. Default setting is 131 LSB/degree/s 
#define DEGREES_50 (131 * 50)           // So every 131 in gyro value equals 1 degree of rotational velocity


uint32_t Addr = 0x3F;                   // I2C address of LCD.
long id = 1, press_id = 1;              // Interrupt count.
long data_time = 0;                     // data point num
long descent_time = 0;                  // Max number of seconds allowed for descent, x 500 because it uses the same 2ms timer

long sum = 0;                           // Sum of accelerometer values
float pressure_sum = 0;                 // Sum of pressure values. 
int16_t average = 0;                    // Moving average variable, accelerometer.
bool collect_flag = 0;                  // flag indicating when to record acceleration sample.
bool wait_flag = 0;                     // flag indicating when to increment interrupt counter.
bool PANIC_flag = 0;                    // flag indicating water is present in housing.
//bool first_test = 1;                  // flag indicating first test(longer countdown)
STATES STATE = WAIT_TO_LAUNCH;                      // Set initial state. 
uint8_t countdown = 0, update_Data = 0;             // Counting variables, one for countdowns, one for updating state to user 
uint8_t RxBuffer[BUFFER_LEN] = {};                  // Rx Buffer
int msg_count = 0, rxflag = 0, bytes = 0, dataflag = 0, transmit_flag = 0;    // UART variables
int depth = 0, reset = 0, testnum = 1;                                                     // Variable depth, reset flag                                              // gyro variables
float xavg = 0, yavg = 0, xsum = 0, ysum = 0;                                 // gyro avg/sum values
char file[11] = "test1.txt";
char volume[10] = {};
FS_FILE *fsfile;

/*******************************************************************************
* Function Name: main
********************************************************************************
*
* Summary:
*  main() performs following functions:
*  1: Initializes all necessary components on board (accelerometer/gyro, SD card, LCD, timers, interrupts, ADC, UART for 
*       Bluetooth).
*  2: Begins at state: WAIT_FOR_LAUNCH. Waits for a bluetooth command to start, then prompts for a desired depth. Upon 
*       completion, starts a countdown for which the device should be thrown in the water before it completes. Switches to 
*       DESCENDING state.
*  3: Samples Z-axis acceleration data from module @ 500hz. Computes moving average of Z-axis acceleration values. Same goes
*       for gyro data in the case that the system flips somehow. If the moving average has breached the threshold(value of 
*       20000), we know it has landed on the bottom. If the time of descent has gone over the max descent time, calculated 
*       from the depth earlier, then we go to resurfacing. 
*  4: At the LANDED state, we delay to let the system settle, then turn on solenoid 1. This solenoid activates the suction
*       in the legs. The suction occurs for 5 seconds, then turns off. Switch to RESURFACING.
*  5: To resurface, we pulse the solenoids at a rate of 3 seconds on to 1 second off. The number of pulses is also determined
*       by the depth. Once the number of pulses has finished, we move to TRANSMIT.
*  6: At TRANSMIT, we simply wait for the data command to begin sending out the collected data or for the reset command to 
*       do another run.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/

int SD_SETUP(char* filename); //SD card setup function

/* Moisture sensor ISR */
CY_ISR (Moisture_ISR_Handler){
    PANIC_flag = 1;                             // Set flag to indicate water
    STATE = RESURFACE;                          // Go to surface
    Comp_Stop();                                // Stop comparator for interrupt
}

/* Sampling ISR */
CY_ISR (Sample_ISR_Handler){
    Sample_Timer_STATUS;                        // Clears interrupt by accessing timer status register
    if (STATE == DESCENDING || STATE == LANDED){
        data_time++;
    }
    collect_flag = 1;
}

/* Countdown ISR*/
CY_ISR (Countdown_ISR_Handler){
    Countdown_timer_STATUS;                        // Clears interrupt by accessing timer status register
    if ((STATE == WAIT_TO_LAUNCH && depth != 0) || STATE == LANDED || STATE == RESURFACE){ 
        wait_flag = 1;
        countdown++;
    }
    #ifdef BT
        if (STATE == TRANSMIT || (STATE == WAIT_TO_LAUNCH && !dataflag)){
            update_Data++;
            if (update_Data == 10){
                transmit_flag = 1;
                update_Data = 0;
            }
        }          
    #endif
}
/* Bluetooth UART Rx ISR*/
CY_ISR(rx_interrupt){
    #ifdef BT
    while (UART_ReadRxStatus() & UART_RX_STS_FIFO_NOTEMPTY){
        RxBuffer[msg_count++] = UART_GetChar();
        if ((msg_count - 3) > bytes)
            rxflag = 1;
    }
    #endif
}

int main()
{
    int num = 0, decimals = 0;                                       // ADC Voltage conversion placeholders
    float voltage = 0, temp = 0, output = 0, pressure_avg = 0;       // ADC Voltage conversion variables
    char buf[50], tempbuf[20] = {}, curState[14] = "SYSTEM_CHECK";  // buffers, UART and initial state
    char descendbuf[DESCENDING_LEN] = STATE_DESCENDING;             // buffers for transmitting states
    char landedbuf[LANDED_LEN] = STATE_LANDED;
    char vacuumbuf[VACUUM_LEN] = STATE_VACUUM;
    char resurfbuf[RESURFACE_LEN] = STATE_RESURFACE;
    char transbuf[TRANSMIT_LEN] = STATE_TRANSMIT;
    int stateMsgCount = 0, pulse = 0, secs_for_tilt = 0;
    
    int16_t ax, ay, az, i;
    int16_t gx, gy, gz;
    int16_t z_offset = 0;
    int tens = 0, ones = 0;                     // digit place variables for message len of bluetooth messages
    
    /* Start the components */
    CYGlobalIntEnable;                          // enable global interrupts
    I2C_Master_Start(); 
    ADC_Start();
    Sample_Timer_Start();                       // start timer module
    Sample_ISR_StartEx(Sample_ISR_Handler);     // reference ISR function
    rx_interrupt_StartEx(rx_interrupt);
    //moisture_isr_StartEx(Moisture_ISR_Handler); // moisture isr start
    //Comp_Start();                               // comparator for moisture start
    UART_Start();
    
    
    #ifdef LCD
        LiquidCrystal_I2C_init(Addr,16,2,0);        // initialize I2C communication with LCD
        begin(); 
    
    #endif
   
    /* initialize MPU6050 */
    #ifdef MPU6050
        MPU6050_init();    
	    MPU6050_initialize(); 
    #endif
        
    #ifdef LCD
        /* Startup Display */
        LCD_print("PSoC 5LP: O-Vac");
        setCursor(0,1);
        LCD_print("I2C Working");
        
        CyDelay(1000u);   
        clear();
    #endif
    
    /* Start the ADC conversion */
    ADC_StartConvert();

    /* Start SD card*/
    #ifdef SD
        int SD_Result = SD_SETUP(file); 
        
    #endif
    
    #ifdef LCD
        /* Display the current State */
        setCursor(0,0);    
        LCD_print(curState);
    #endif
    STATE = WAIT_TO_LAUNCH;
    #ifdef LCD
        setCursor(0,0);
        clear();
        LCD_print("STATE: WAIT");  
    #endif
    
    Countdown_timer_Start();
    countdown_StartEx(Countdown_ISR_Handler);
   
    
    for(;;)
    {
        
        if(ADC_IsEndConversion(ADC_RETURN_STATUS))              // voltage conversion for pressure
        {
            output = ADC_GetResult32();
            voltage = output * (3.32 / 4096);
            if(collect_flag == 1){
                if (press_id < MA_WINDOW){
                    pressure_sum += voltage;     
                }
                else if(press_id == MA_WINDOW){
                    pressure_sum += voltage;
                    pressure_avg = pressure_sum/MA_WINDOW;                            // compute baseline average
                }
                else{
                    pressure_avg = ComputeMA(pressure_avg, MA_WINDOW, voltage);
                    num = pressure_avg;
                    temp = pressure_avg - num;
                    decimals = temp * 10000;
                    char sdbuf[60] = {};
                    #ifdef SD
                        sprintf(sdbuf, "%d.%04d, %d, %d\n", num, decimals, (int16)output, average); // log pressure data
                        FS_Write(fsfile, sdbuf, strlen(sdbuf));                           
                    #endif 
                }
                if (STATE != DESCENDING) collect_flag = 0;
                press_id++;
            }
        }
        
       
        
        
    /* Bluetooth message response, after 2 bytes received, retrieve message from those 2 bytes. Once full message has
     * has arrived, process it. */
    #ifdef BT
        if (msg_count >= 2){
            tens = RxBuffer[0] - 48;
            ones = RxBuffer[1] - 48;
            bytes = (tens * 10) + ones;
        } 
        
        if(rxflag) {
            uint8_to_char(RxBuffer, &tempbuf[0], 20);
            depth = BT_Process(&tempbuf[3], &STATE, bytes, &dataflag, &reset);
            
            msg_count = 0; bytes = 0;
            memset(RxBuffer, 0, BUFFER_LEN);
            memset(tempbuf, 0, 20);
            countdown = 0;
            rxflag = 0;
        }
    #endif
    
        /* Get Z-Acceleration */

        az = MPU6050_getAccelerationZ();

        int t = 1;
        
        
         if(collect_flag == 1){              // Check accelerometer and gyro data
                    if (id < MA_WINDOW){    
                        sum += az;  
                    }
                    else if(id == MA_WINDOW){
                        sum += az;
                        average = sum/MA_WINDOW;                          
                    }
                    else{
                        average = ComputeMA(average, MA_WINDOW, az);                // Compute averages for gyro
                    }
                    id++;
                    
        }
        /* State Machine */
        switch (STATE){
    
            /* Waiting for start command and depth*/
            case WAIT_TO_LAUNCH:  
                if (reset){                         // If reset command was received, reset:
                    id = 1;                                // Interrupt count.
                    data_time = 0;                         // data point num
                    sum = 0;                               // Sum of accelerometer values. 
                    average = 0;                           // Moving average variable.
                    xavg = 0; yavg = 0;                    // Gyro average variables
                    collect_flag = 0;                      // flag indicating when to record acceleration sample.
                    wait_flag = 0;                         // flag indicating when to increment interrupt counter.
                    PANIC_flag = 0;                        // flag indicating water is present in housing.
                    //bool first_test = 1;                 // flag indicating first test(longer countdown)
                    depth = 0; countdown = 0;              // Current desired depth, variable for counting seconds 
                    msg_count = 0; dataflag = 0;           // BT message len variable, data flag 
                    reset = 0;                             // indicates whether to reset variables or not
                    pulse = 0;
                    #ifdef LCD
                        setCursor(0,0);
                        clear();
                        LCD_print("STATE: WAIT");  
                    #endif 
                }
            
                if (transmit_flag){
                    BT_Send(&tempbuf[0], &STATE, 10, &tens); // Here, the STATE variable only matters, rest do not matter(could be anything)
                    transmit_flag = 0;
                }
                // Once depth has been entered, can begin countdown into descending
                if(wait_flag == 1){
                    #ifdef BT
                        stateMsgCount = 0;
                        sprintf(buf, "\n%d seconds remaining", (10 - countdown));
                        while (stateMsgCount < 21){
                            while (UART_ReadTxStatus() & UART_TX_STS_FIFO_NOT_FULL){
                                UART_PutChar(buf[stateMsgCount++]);
                                if (stateMsgCount >= 21) break;
                            }
                        }
                    #endif
                    /* at 10 seconds, change into descending */
                    if(countdown == 10){
                        descent_time = (((depth / 13) + 3) * 2 * 500);
                        /* descent time takes about 2~3 seconds to go 13 feet, add 3 for extra 10m of leeway, x500 for
                         * number of ISR calls to get 1 second */ 
                        STATE = DESCENDING;
                        #ifdef LCD
                            setCursor(0,0);
                            clear();
                            LCD_print("STATE: DESCENT");
                        #endif
                        #ifdef BT
                            stateMsgCount = 0;
                            while (stateMsgCount < DESCENDING_LEN){
                                while (UART_ReadTxStatus() & UART_TX_STS_FIFO_NOT_FULL){
                                    UART_PutChar(descendbuf[stateMsgCount++]);
                                    if (stateMsgCount >= DESCENDING_LEN) break;
                                }
                            }
                        #endif
                        countdown = 0; 
                    }
                    wait_flag = 0; 
                }
                break;
                
            case DESCENDING:
                

         if(collect_flag == 1){              // Check accelerometer and gyro data
                    if(average > BOT_THRESHOLD){                        
                        STATE = LANDED;                                     //Switch to LANDED state 
                        #ifdef LCD
                            setCursor(0,0);
                            clear();
                            LCD_print("STATE: LANDED");  
                        #endif
                        #ifdef SD
                            FS_Write(fsfile, landedbuf, LANDED_LEN);
                        #endif
                        #ifdef SD
                            FS_Write(fsfile, vacuumbuf, VACUUM_LEN);
                        #endif
                        
                        
                        countdown = 0;
                    }
                    
                    
                    /* if max time allowed for descent has been reached, resurface */
                    if(data_time >= descent_time ){                         // variable descent time
                        STATE = RESURFACE;                                      
                        #ifdef LCD
                            setCursor(0,0);
                            clear();
                            LCD_print("STATE: RESURFACE");  
                        #endif
                        id=0;                                               //reset sample counter
                        data_time = 0;
                        sum = 0;                                            //reset sum 
                        average = 0;
                    }
                    collect_flag = 0;
                    
                }
                break;
                
                case LANDED:
                    if (countdown == 7 && !pulse) {                   // Delay for 7 seconds at bottom
                        countdown = 0; 
                        pulse = 1;                          // next stage of the state
                        Solenoid_1_Write(1);                // turn on solenoid 1 for 5 seconds
                    } 
                    
                    if ((countdown == 15) && pulse){           // Second stage, turn off solenoid
                        pulse++;
                        Solenoid_1_Write(0);                // turn off soleniod 1
                        countdown = 0;
                    }
                    if (countdown == 3 && pulse == 2){      // Delay for 3 seconds then resurface
                        STATE = RESURFACE;
                        
                        #ifdef LCD
                            setCursor(0,0);
                            clear();
                            LCD_print("STATE: RESURFACING");  
                        #endif
                        #ifdef SD
                            FS_Write(fsfile, resurfbuf, RESURFACE_LEN);
                        #endif
                        pulse = 0;
                        countdown = 0;
                    }
                break;
                
            case RESURFACE:
                if (PANIC_flag)                 // Display that moisture sensor triggered
                    LCD_print("WATER DETECTED");
                    
                Solenoid_2_Write(1);            // turn on lift bag solenoid                
                
                //check pressure sensor to confirm we are at the surface
                if (countdown == 3){
                    Solenoid_2_Write(0);        // Turn off solenoid 2 for 1 second
                    CyDelay(1000u);
                    pulse++;
                    countdown = 0;
                }
                if (pulse == 4){
                    STATE = TRANSMIT;
                    #ifdef SD                                   //close old file, open new one
                        FS_FClose(fsfile);
                        sprintf(file, "test%d.txt", ++testnum);
                        fsfile = FS_FOpen(file, "w");
                    #endif 
                    
                    #ifdef LCD
                        setCursor(0,0);
                        clear();
                        LCD_print("TRANSMIT");  
                    #endif
                    #ifdef SD
                        FS_Write(fsfile, transbuf, TRANSMIT_LEN);
                    #endif
                    countdown = 0;
                }
                break;
                
            case TRANSMIT:
                if (transmit_flag){
                    BT_Send(&tempbuf[0], &STATE, 10, &t); // Here, the STATE variable only matters, rest do not matter(could be anything)
                    transmit_flag = 0;
                }            
                break;
                
            default:
                break;
        
        }
    }
}

int SD_SETUP(char* filename){
int success = 1;
      FS_Init();
            FS_Mount(volume);
            if(0 != FS_GetVolumeName(0u, volume, 9u))
                /* Getting volume name succeeded so prompt it on the LCD */
                LCD_print("Sd vol succeed");
            else{
                LCD_print("Sd vol failed");
                success = 0;
            }
            CyDelay(500u);
            clear();
            if(0 == FS_FormatSD(volume))
                LCD_print("format Succeeded");
            else{
                LCD_print("format Failed");
                success = 0;
            }
            
            CyDelay(500u);
            clear();
            
            fsfile = FS_FOpen(filename, "w");
            if(fsfile)
            {
                /* Indicate successful file creation message */
                LCD_print("File ");
                LCD_print("was opened");
                /* Need some delay to indicate output on the LCD */
                CyDelay(500u);
                clear();
                
                if(0 != FS_Write(fsfile, filename, strlen(filename))) 
                    /* Inditate that data was written to a file */
                    LCD_print("written to file");
                else {
                    LCD_print("Failed to write");
                    success = 0;
                    clear();
                }
                CyDelay(500u);
            }
            else{
                LCD_print("file not created");
                success = 0;
            }
        FS_Write(fsfile, "\n------------\n", 14);
return success;
}


/* [] END OF FILE */
