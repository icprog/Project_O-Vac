/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "LiquidCrystal_I2C.h"
#include "functions.h"
#include "mpu6050.h"

#define MA_WINDOW 15                    // Number of samples in the moving average window.
#define BUFFER_LEN  64u                 // Buffer length for UART rx

uint32_t Addr = 0x3F;                       // I2C address of LCD.
long id = 1, press_id = 1;                 // Interrupt count.
bool collect_flag = 0;                         // flag indicating when to increment interrupt counter.
uint8_t RxBuffer[BUFFER_LEN] = {};            // Rx Buffer
int msg_count = 0, rxflag = 0, bytes = 0, reset;    // UART variables
int dummy = 0;
float pressure_sum = 0;                     // Sum of pressure values.
char file[11] = "test_2.txt";           // Pressure Variables
char volume[10] = {};
FS_FILE *fsfile;

int SD_SETUP(char* filename); //SD card setup function

/* Sampling ISR */
CY_ISR (Sample_ISR_Handler){
    Sample_Timer_STATUS;                        // Clears interrupt by accessing timer status register
    collect_flag = 1;
}

CY_ISR(rx_interrupt){
    while (UART_ReadRxStatus() & UART_RX_STS_FIFO_NOTEMPTY){
        RxBuffer[msg_count++] = UART_GetChar();
        if ((msg_count - 3) >= bytes)
            rxflag = 1;
    }
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    int num = 0, decimals = 0;                                       // ADC Voltage conversion placeholders
    float voltage = 0, temp = 0, output = 0, pressure_avg = 0;       // ADC Voltage conversion variables
    char tempbuf[70] = {};
    int tens = 0, ones = 0;                     // digit place variables for message len of bluetooth messages
    int16_t gyroX = 0, gyroY = 0;
    
    //I2C_Master_Start(); 
    ADC_Start();
    Sample_Timer_Start();                       // start timer module
    Sample_ISR_StartEx(Sample_ISR_Handler);     // reference ISR function
    //rx_interrupt_StartEx(rx_interrupt);
    UART_Start();
    //LiquidCrystal_I2C_init(Addr,16,2,0);        // initialize I2C communication with LCD
    begin();
    
    ADC_StartConvert();

    /* Start SD card*/
    int SD_Result = SD_SETUP(file);
    if (!SD_Result) {;}//LCD_print("SD_failed"); CyDelay(2000u);}
    
    char run_separation[40] = "RUN COMPLETED\n"
                              "*\n"
                              "*\n"
                              "*\n"
                              "NEXT RUN START\n";
    
    //MPU6050_init();
    //MPU6050_initialize();
    clear();
    float xavg = 0, yavg = 0, zavg = 0, xsum = 0, ysum = 0, zsum = 0;
    float xfavg = 0, yfavg = 0, zfavg = 0;
    int xones = 0, xdecimals = 0, yones = 0, ydecimals = 0, zones = 0, zdecimals = 0;
    for(;;)
    {
        CyDelay(50);
        clear();
        //MPU6050_getRotation(&gyroX, &gyroY, &gyroZ);
        if (reset){
            FS_Write(fsfile, run_separation, strlen(run_separation));
            reset = 0;
        }
//        if (collect_flag){
//            
//            if (press_id < MA_WINDOW){
//                    xsum += gyroX;
//                    ysum += gyroY;
//                    zsum += gyroZ;
//                }
//                else if(press_id == MA_WINDOW){
//                    xsum += gyroX;
//                    ysum += gyroY;
//                    zsum += gyroZ;
//                    xavg = xsum/MA_WINDOW;                            // compute baseline average
//                    yavg = ysum/MA_WINDOW;
//                    zavg = zsum/MA_WINDOW;
//                }
//                else{
//                    xavg = ComputeMA(xavg, MA_WINDOW, gyroX);
//                    yavg = ComputeMA(yavg, MA_WINDOW, gyroY);
//                    zavg = ComputeMA(zavg, MA_WINDOW, gyroZ);
//                    xfavg = xavg / 131; yfavg = yavg / 131; zfavg = zavg / 131;
//                    xones = xfavg; yones = yfavg; zones = zfavg;
//                    xdecimals = ((xones - xfavg) * 1000);
//                    ydecimals = ((yones - yfavg) * 1000);
//                    zdecimals = ((zones - zfavg) * 1000);
//                   
//                        //sprintf(tempbuf, "%d.%d -%d.%d %d.%d\n", xones, xdecimals, yones, (ydecimals * -1), zones, zdecimals);}
//             
//                    sprintf(tempbuf, "%d.%d %d.%d %d.%d\n", xones, xdecimals, yones, ydecimals, zones, zdecimals);
//                    FS_Write(fsfile, tempbuf, strlen(tempbuf));                            
//                }
//            press_id++;
//            collect_flag = 0;
//        }
        if(ADC_IsEndConversion(ADC_RETURN_STATUS))
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

                        sprintf(sdbuf, "(%ld)pressure: %d.%04d, %d\n", press_id, \
                                            num, decimals, (int16)output); // log pressure data
                        FS_Write(fsfile, sdbuf, strlen(sdbuf));                            
                }
                collect_flag = 0;
                press_id++;
            }  
        }
        if (msg_count >= 2){
            tens = RxBuffer[0] - 48;
            ones = RxBuffer[1] - 48;
            bytes = (tens * 10) + ones;
        } 
        
        if(rxflag) {
            uint8_to_char(RxBuffer, &tempbuf[0], 20);
            dummy = BT_Process(&tempbuf[3], bytes, &reset);
            
            msg_count = 0; bytes = 0;
            memset(RxBuffer, 0, BUFFER_LEN);
            memset(tempbuf, 0, 20);
            rxflag = 0;
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
                return success;
            }
            CyDelay(500u);
            clear();
            if(0 == FS_FormatSD(volume))
                ;
            else{
                LCD_print("format Failed");
                success = 0;
                return success;
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
                    return success;
                }
                CyDelay(500u);
            }
            else{
                LCD_print("file not created");
                success = 0;
                return success;
            }
        FS_Write(fsfile, "\n------------\n", 14);
return success;
}

/* [] END OF FILE */
