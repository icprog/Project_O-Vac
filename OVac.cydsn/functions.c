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
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "functions.h"
#include "LiquidCrystal_I2C.h"

char accelData[10];

void I2C_LCD_print(uint8_t row, uint8_t column, uint16_t ax, uint16_t ay,uint16_t az){
    sprintf(accelData,"%d %d %d     ",ax,ay,az); 
    //clear();
    setCursor(column,row);
    LCD_print(accelData);
}

int16_t ComputeMA(int16_t avg, int16_t n, int16_t sample){
    avg -= avg/n;
    avg += sample/n;
    return avg;    
}

void Process_BT(uint8_t *RxBuffer, STATES *STATE, int bytes){
    int i = 0;
    if (strncmp(*RxBuffer, "start", 5)){
        *STATE = DESCENDING;
    }
    while (i < bytes + 3){
        while(UART_ReadTxStatus() & UART_TX_STS_FIFO_NOT_FULL){
            UART_PutChar(RxBuffer[i++]);
        }
    }
}

/* [] END OF FILE */
