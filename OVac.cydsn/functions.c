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
int BT_Process(char *RxBuffer, STATES *STATE, int bytes, int *flag, int *reset){
    int i = 0;
    int ones = 0, tens = 0, hunds = 0;
    char commands[COMMANDS_LEN] = COMMANDS;
    char depth[SEND_DEPTH_LEN] = SEND_DEPTH;
    
    if (!strncmp(RxBuffer, "reset", 5)){                      // stop/reset program, go back to WAIT
        *STATE = WAIT_TO_LAUNCH;
        *reset = 1;
    } else if (*STATE == WAIT_TO_LAUNCH && !strncmp(RxBuffer, "start", 5)){ // start the process, ask for depth
         *flag = 1;
        while (i < SEND_DEPTH_LEN){
            while(UART_ReadTxStatus() & UART_TX_STS_FIFO_NOT_FULL)
                UART_PutChar(depth[i++]);
        }
    } else if (*STATE == WAIT_TO_LAUNCH && !strncmp(RxBuffer, "d:", 2)){
        hunds = RxBuffer[2] - 48;
        tens = RxBuffer[3] - 48;
        ones = RxBuffer[4] - 48;
         
        if (hunds){ UART_PutChar(RxBuffer[2]); UART_PutChar(RxBuffer[3]);}
        else if (tens) UART_PutChar(RxBuffer[3]);
        UART_PutChar(RxBuffer[4]);
        
        return (hunds * 100) + (tens * 10) + ones;
    } else if (*STATE == TRANSMIT && !strncmp(RxBuffer, "data", 4)){ // prompt to start sending data back
        *flag = 1;
    } else {
        while (i < COMMANDS_LEN){
            while(UART_ReadTxStatus() & UART_TX_STS_FIFO_NOT_FULL)
                UART_PutChar(commands[i++]);
        }
        return (hunds * 100) + (tens * 10) + ones;
    }
   
    while (i < bytes + 3){
        while(UART_ReadTxStatus() & UART_TX_STS_FIFO_NOT_FULL)
            UART_PutChar(RxBuffer[i++]);
    }
    return (hunds * 100) + (tens * 10) + ones;
}

/* For sending status of State, or data back in transmit state */
void BT_Send(char *TxBuffer, STATES *STATE, int lengthOfBuf, int *firstPacket){
    int i = 0;
    uint8 waitstate[WAITING_LEN] = STATE_WAITING;
    uint8 transtate[TRANSMITTING_LEN] = TRANSMITTING;
    
    if (*STATE == WAIT_TO_LAUNCH){                  // Just send STATE back to indicate
        while (i < WAITING_LEN - 1){
            while(UART_ReadTxStatus() & UART_TX_STS_FIFO_NOT_FULL)
                UART_PutChar(waitstate[i++]);
        }
    }
    else if (*STATE == TRANSMIT){                   // Send STATE back then data from buffer
        if (*firstPacket){                          // only send STATE once
            while (i < TRANSMITTING_LEN - 1){
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
