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
#include <string.h>
#include <FS.h>
#include "LiquidCrystal_I2C.h"
#include "functions.h"

//#define MPU6050 
//#define LCD
//#define SD
#define BT

#define MA_WINDOW 15                    // Number of samples in the moving average window.
#define BOT_THRESHOLD 20000             // Z-Aacceleration threshold for transition into LANDED state.
#define WAIT_TIME 1000                  // Number of ISR calls until transition into DESCENDING state.
#define DATA_TIME 5000                  // Number of ISR calls until transition into WAIT_TO_LAUNCH state.
#define BUFFER_LEN  64u                 // Buffer length for UART rx


uint32_t Addr = 0x3F;                       // I2C address of LCD.
long id = 1;                                // Interrupt count.
long data_time = 0;                        // data point num

long sum = 0;                               // Sum of accelerometer values. 
int16_t average = 0;                        // Moving average variable.
bool collect_flag = 0;                      // flag indicating when to record acceleration sample.
bool wait_flag = 0;                         // flag indicating when to increment interrupt counter.
bool PANIC_flag = 0;                        // flag indicating water is present in housing.
//bool first_test = 1;                        // flag indicating first test(longer countdown)
STATES STATE = WAIT_TO_LAUNCH;         // Set initial state. 
uint8_t testnum = 1, countdown = 0, update_Data = 0;
uint8_t RxBuffer[BUFFER_LEN] = {};            // Rx Buffer
int msg_count = 0, rxflag = 0, bytes = 0, dataflag = 0, transmit_flag = 0;    // UART variables
char file[11] = "test_1.txt";
char volume[10] = {};
FS_FILE *fsfile;

/*******************************************************************************
* Function Name: main
********************************************************************************
*
* Summary:
*  main() performs following functions:
*  1: Initializes the LCD.
*  2: Initializes timer module and sampling interrupt.
*  3: Initializes MPU6050 Accelerometer/Gyroscope module.
*  4: Samples Z-axis acceleration data from module @ 500hz.
*  5: Computes moving average of Z-axis acceleration values.
*  6: Transitions from DESCENDING to LANDED state when sudden acceleration occurs
*     (ie. moving average > 200000).
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/


/* Moisture sensor ISR */
CY_ISR (Moisture_ISR_Handler){
    PANIC_flag = 1;                             // Set flag to indicate water
    STATE = RESURFACE;                          // Go to surface
    Comp_Stop();                                // Stop comparator for interrupt
}

/* Sampling ISR */
CY_ISR (Sample_ISR_Handler){
    
    Sample_Timer_STATUS;                        // Clears interrupt by accessing timer status register
    if (STATE == DESCENDING){ 
        collect_flag = 1;
        data_time++;
    }
}

/* Countdown ISR*/
CY_ISR (Countdown_ISR_Handler){
    
    Countdown_timer_STATUS;                        // Clears interrupt by accessing timer status register
    if (STATE == WAIT_TO_LAUNCH){ 
        wait_flag = 1;
        countdown++;
    }
    else  if (STATE == DESCENDING){
        wait_flag = 1;
    }
    #ifdef BT
        if (STATE == TRANSMIT || STATE == WAIT_TO_LAUNCH){
            update_Data++;
            if (update_Data == 4){
                transmit_flag = 1;
                update_Data = 0;
            }
        }
            
    #endif
}

CY_ISR(rx_interrupt){
    while (UART_ReadRxStatus() & UART_RX_STS_FIFO_NOTEMPTY){
        RxBuffer[msg_count++] = UART_GetChar();
        if ((msg_count - 3) == bytes)
            rxflag = 1;
    }
}

int main()
{
    LED2_1_Write(1); 
    float output;
    char buf[50];                               // just to hold text values in for writing to UART
    char tempbuf[20] = {};
    
    char curState[14] = "SYSTEM_CHECK  ";
    int16_t ax, ay, az, i;
    //int16_t gx, gy, gz;
    int16_t z_offset = 0;
    int tens = 0, ones = 0;                     // digit place variables for message len of bluetooth messages
    
    /* Start the components */
    CYGlobalIntEnable;                          // enable global interrupts
    //I2C_Master_Start(); 
    //ADC_Start();
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
        {
            FS_Init();
            FS_Mount(volume);
            if(0 != FS_GetVolumeName(0u, volume, 9u))
                /* Getting volume name succeeded so prompt it on the LCD */
                LCD_print("Sd vol succeed");
            else
                LCD_print("Sd vol failed");
                
            CyDelay(500u);
            clear();
            if(0 == FS_FormatSD(volume))
                LCD_print("format Succeeded");
            else
                LCD_print("format Failed");
          
            CyDelay(500u);
            clear();
            
            fsfile = FS_FOpen(file, "w");
            if(fsfile)
            {
                /* Indicate successful file creation message */
                LCD_print("File ");
                LCD_print("was opened");
                /* Need some delay to indicate output on the LCD */
                CyDelay(500u);
                clear();
                
                if(0 != FS_Write(fsfile, file, strlen(file))) 
                    /* Inditate that data was written to a file */
                    LCD_print("written to file");
                else
                    LCD_print("Failed to write");
                    clear();
                CyDelay(500u);
            }
            else
                LCD_print("file not created");
        }
        FS_Write(fsfile, "\n------------\n", 14);
    #endif
    
    #ifdef LCD
        /* Display the current State */
        setCursor(0,0);    
        LCD_print(curState);
    #endif
    
    
    Countdown_timer_Start();
    countdown_StartEx(Countdown_ISR_Handler);
    int num = 0, decimals = 0;
    float voltage = 0, temp = 0;
    
    
    for(;;)
    { 
          
        if(ADC_IsEndConversion(ADC_RETURN_STATUS))
        {
            char pressure[50];
            output = ADC_GetResult32();
            
            setCursor(0, 1);
            voltage = output * (3.32 / 4096);
            num = voltage;
            temp = voltage - num;
            decimals = temp * 10000;
            #ifdef SD
//                
            #endif
            
        }
        
    /* Bluetooth message response*/
    #ifdef BT
        if (msg_count == 2){
            tens = RxBuffer[0] - 48;
            ones = RxBuffer[1] - 48;
            bytes = (tens * 10) + ones;
        } 
        
        if(rxflag) {
            uint8_to_char(RxBuffer, &tempbuf[0], 20);
            BT_Process(&tempbuf[3], &STATE, bytes, &dataflag);
            msg_count = 0; bytes = 0;
            memset(RxBuffer, 0, BUFFER_LEN);
            rxflag = 0;
        }
    #endif
    
        /* Display Z-Acceleration */
        //clear();
        //az = MPU6050_getAccelerationZ();
       // I2C_LCD_print(1,0, id ,0,average);                                //print Interrupt count and Z-Acceleration
        
        /* State Machine */
        switch (STATE){
    
            case WAIT_TO_LAUNCH:
                id = 1;                                // Interrupt count.
                data_time = 0;                        // data point num

                sum = 0;                               // Sum of accelerometer values. 
                average = 0;                        // Moving average variable.
                collect_flag = 0;                      // flag indicating when to record acceleration sample.
                wait_flag = 0;                         // flag indicating when to increment interrupt counter.
                PANIC_flag = 0;                        // flag indicating water is present in housing.
                //bool first_test = 1;                        // flag indicating first test(longer countdown)
                testnum = 1, countdown = 0;
                if (transmit_flag){
                    BT_Send(&tempbuf[0], &STATE, 10, &tens); // Here, the STATE variable only matters, rest do not matter(could be anything)
                    transmit_flag = 0;
                }
                if(wait_flag == 1){
                    #ifdef LCD
                        setCursor(0,0);
                        clear();
                        
                        sprintf(buf, "T-minus %d seconds\n", (60 - countdown)); // countdown
                        LCD_print(buf);
                    #endif
//                    if(countdown == 60){
//                        STATE = DESCENDING;
//                        #ifdef LCD
//                            setCursor(0,0);
//                            clear();
//                            LCD_print("STATE: DESCENT");
//                        #endif           
//                        id=0;
//                        data_time = 0;
//                        countdown = 9;
//                        LED2_Write(1);                                      // turn the LED off 
//                    }
                    wait_flag = 0; 
                }
                break;
                
            case DESCENDING:
                STATE = TRANSMIT;
                if(collect_flag == 1){
                    
                    if (id<MA_WINDOW){
                        sum += az;     
                    }
                    else if(id == MA_WINDOW){
                        sum += az;
                        average = sum/MA_WINDOW;                            //compute baseline average
                    }
                    else{
                        average = ComputeMA(average, MA_WINDOW, az);
                        /*sprintf(buf, "%d", average);
                        setCursor(0,1);
                        LCD_print(buf);*/
                        
                    }
                    
                    if(average > BOT_THRESHOLD){
                        LED2_Write(0);
                        LED7_Write(1);                                          //turn LED on                        
                        STATE = LANDED;                                     //Switch to LANDED state 
                        #ifdef LCD
                            setCursor(0,0);
                            clear();
                            LCD_print("STATE: LANDED");  
                        //I2C_LCD_print(1,0, id ,0,average);
                        #endif
                        char sdbuf[60] = {};
                        #ifdef SD
                            sprintf(sdbuf, "STATE: LANDED ***********\n");
                            FS_Write(fsfile, sdbuf, strlen(sdbuf));
                        #endif
                        id=0;                                                   //reset sample counter
                        data_time = 0;
                        sum = 0;
                        average = 0;                
                    }
                    id++;     
                    
                    /*if desired amount of samples have been collected, switch states*/
                    if(data_time >= DATA_TIME){
                        LED2_Write(0);                                          //turn LED off
                                                            
                        STATE = WAIT_TO_LAUNCH;                                //Switch to Waiting state    
                        #ifdef LCD
                            setCursor(0,0);
                            clear();
                            LCD_print("STATE: WAIT");  
                        #endif
                        id=0;                                                  //reset sample counter
                        data_time = 0;
                        sum = 0;                                               //reset sum 
                        average = 0;
                       
                    }
                    collect_flag = 0;
                }
                else if(wait_flag == 1){
                       char sdbuf[60] = {};
                        #ifdef SD
                            sprintf(sdbuf, "pressure: %d.%04d, %d\n", num, decimals, (int16)output);
                            FS_Write(fsfile, sdbuf, strlen(sdbuf));                           
                        #endif 
                        wait_flag = 0;
                }
                break;
                
                case LANDED:
                    CyDelay(5000u);
                    Solenoid_1_Write(1); //turn on solenoid 1
                    #ifdef LCD
                        setCursor(0,0);
                        clear();
                        LCD_print("VACUUMING");  
                    #endif
                    char sdbuf[60] = {};
                    #ifdef SD
                            memset(sdbuf, 0, 40);
                            sprintf(sdbuf, "STATE: VACUUMING ***********\n");
                            FS_Write(fsfile, sdbuf, strlen(sdbuf));
                    #endif
                    CyDelay(7000u);
                    Solenoid_1_Write(0); //turn off soleniod 1
                    CyDelay(5000u);
                    STATE = RESURFACE;
                    #ifdef LCD
                        setCursor(0,0);
                        clear();
                        LCD_print("STATE: RESURFACING");  
                    #endif
                    #ifdef SD
                            memset(sdbuf, 0, 40);
                            sprintf(sdbuf, "STATE: RESURFACING ***********\n");
                            FS_Write(fsfile, sdbuf, strlen(sdbuf));
                    #endif
                break;
                
            case RESURFACE:
                //CyDelay(4000u);
                if (PANIC_flag)
                    LCD_print("WATER DETECTED");
                Solenoid_2_Write(1); //turn on solendiod 2
                CyDelay(15000u);
                Solenoid_2_Write(0); //turn off solenoid 2
                //check pressure sensor to confirm we are at the surface
                CyDelay(5000u);                                //wait 10 seconds to lift, for testing in pool
                STATE = TRANSMIT;
                #ifdef LCD
                    setCursor(0,0);
                    clear();
                    LCD_print("TRANSMIT");  
                #endif
                #ifdef SD
                    memset(sdbuf, 0, 40);
                    sprintf(sdbuf, "STATE: TRANSMIT ***********\n");
                    FS_Write(fsfile, sdbuf, strlen(sdbuf));
                #endif
                break;
            case TRANSMIT:
                LED2_Write(1);
                LED7_Write(1);
                int t = 1;
                if (transmit_flag){
                    BT_Send(&tempbuf[0], &STATE, 10, &t); // Here, the STATE variable only matters, rest do not matter(could be anything)
                    transmit_flag = 0;
                }
                #ifdef SD                                   //close old file, open new one
                    FS_FClose(fsfile);
                    sprintf(file, "test%d.txt", ++testnum);
                    fsfile = FS_FOpen(file, "w");
                #endif 
                CyDelay(15000u);
                
//            case LANDED:
//                CyDelay(1000u);                             // delay a bit
//                LED2_Write(1);                              // turn on other LED to indicate landed
//                STATE = WAIT_TO_LAUNCH;
//                #ifdef LCD
//                    setCursor(0,0);
//                    clear();
//                    LCD_print("STATE: WAIT");
//                #endif 
//                #ifdef SD                                   // close old file, open new one
//                    FS_FClose(fsfile);
//                    sprintf(file, "test%d.txt", ++testnum);
//                    fsfile = FS_FOpen(file, "w");
//                #endif 
                break;
                
            default:
                break;
        
        
        }
        
    }
}


/* [] END OF FILE */
