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

float ComputeMA(float avg, int16_t n, float sample){
    avg -= avg/n;
    avg += sample/n;
    return avg;    
}

/* Process an incoming message, only for WAIT, TRANSMIT states, or for a reset of the system */
void BT_Process(char *RxBuffer, STATES *STATE, int bytes, int *dataflag){
    int i = 0;
    char commands[COMMANDS_LEN] = COMMANDS;
    if (!strncmp(RxBuffer, "stop", 4)){                      // stop/reset program, go back to WAIT
        *STATE = WAIT_TO_LAUNCH;
    } else if (*STATE == WAIT_TO_LAUNCH && !strncmp(RxBuffer, "start", 5)){ // start the process, go to DESCENDING
        *STATE = DESCENDING;
    } else if (*STATE == TRANSMIT && !strncmp(RxBuffer, "data", 4)){ // prompt to start sending data back
        *dataflag = 1;
    } else{
        while (i < COMMANDS_LEN){
            while(UART_ReadTxStatus() & UART_TX_STS_FIFO_NOT_FULL)
                UART_PutChar(commands[i++]);
        }
        return;
    }
   
    while (i < bytes + 3){
        while(UART_ReadTxStatus() & UART_TX_STS_FIFO_NOT_FULL)
            UART_PutChar(RxBuffer[i++]);
    }
}

/* For sending status of State, or data back in transmit state */
void BT_Send(char *TxBuffer, STATES *STATE, int lengthOfBuf, int *firstPacket){
    int i = 0;
    uint8 waitstate[23] = "STATE = wait_to_launch";
    uint8 transtate[17] = "STATE = transmit";
    
    if (*STATE == WAIT_TO_LAUNCH){                  // Just send STATE back to indicate
        while (i < 23){
            while(UART_ReadTxStatus() & UART_TX_STS_FIFO_NOT_FULL)
                UART_PutChar(waitstate[i++]);
        }
    }
    else if (*STATE == TRANSMIT){                   // Send STATE back then data from buffer
        if (*firstPacket){                          // only send STATE once
            while (i < 17){
                while(UART_ReadTxStatus() & UART_TX_STS_FIFO_NOT_FULL)
                    UART_PutChar(transtate[i++]);
            }
            *firstPacket = 0;
        } i = 0;
//        while (i < lengthOfBuf - 1){                // Send buffer 
//            while(UART_ReadTxStatus() & UART_TX_STS_FIFO_NOT_FULL)
//                UART_PutChar(TxBuffer[i++]);
//        }
    }
}

/* Convert uint8 array to chars, for type warnings/errors in above functions*/
void uint8_to_char(uint8_t a[], char *b, int len){
    int i = 0;
    for (;i < len; i++){
        b[i] = (char)a[i];
    }
}
/* [] END OF FILE */
