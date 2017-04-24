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


/*******************************************************************************
* Function Name: main
********************************************************************************
*
* Summary:
*  main() performs following functions:
*  1: Initializes the LCD
*  2: Starts ADC
*  3: Starts ADC converstion.
*  4: Gets the converted result and displays it in LCD.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <mpu6050.h>

#define MA_WINDOW 15
#define BOT_THRESHOLD 20000

int flag = 0;

CY_ISR(timer_handle){
    flag = 1;
    Timer_STATUS;
}

int16_t ComputeMA(int16_t avg, int16_t n, int16_t sample){
    avg -= avg/n;
    avg += sample/n;
    return avg;    
}

long sum = 0;                               // Sum of accelerometer values. 
int16_t average = 0;                        // Moving average variable.
long id = 1;                                // Interrupt count.

int main()
{
    int16_t ax, ay, az, i;

    /* Start the components */
    CYGlobalIntEnable; 
    I2C_Master_Start();
    Timer_Start();
    ISR_StartEx(timer_handle);
    LCD_Start();

    MPU6050_init();    
	MPU6050_initialize();
    /* Display the value of ADC output on LCD */

    char test[20] = {};
    for(;;)
    {
        az = MPU6050_getAccelerationZ();
        if(flag == 1){
                    
                    if (id<MA_WINDOW){
                        sum += az;     
                    }
                    else if(id == MA_WINDOW){
                        sum += az;
                        average = sum/MA_WINDOW;                            //compute baseline average
                    }
                    else{
                        average = ComputeMA(average, MA_WINDOW, az);
                        #ifdef SD
                            memset(sdbuf, 0, 40);
                            sprintf(sdbuf, "Time%ld: Z-value: %d\n", data_point, average);
                            FS_Write(fsfile, sdbuf, strlen(sdbuf));
                        #endif
                        LCD_Position(0,1);
                        LCD_PrintDecUint16(average);
                    }
                    
          

                    if(average > BOT_THRESHOLD){
                        CyDelay(2000u);
                        #ifdef LCD
                            setCursor(0,0);
                            clear();
                            LCD_print("STATE: LANDED");  
                        //I2C_LCD_print(1,0, id ,0,average);
                        #endif
                        #ifdef SD
                            memset(sdbuf, 0, 40);
                            sprintf(sdbuf, "STATE: LANDED ***********\n");
                            FS_Write(fsfile, sdbuf, strlen(sdbuf));
                        #endif
                        id=0;                                                   //reset sample counter
                        sum = 0;
                        average = 0;                
                    }     
                    id++;
                    /*if desired amount of samples have been collected, switch states*/
                }
    }
}


/* [] END OF FILE */
