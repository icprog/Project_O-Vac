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

float ComputeMA(float avg, int16_t n, float sample){
    avg -= avg/n;
    avg += sample/n;
    return avg;    
}

/* Process an incoming message, only for WAIT, TRANSMIT states, or for a reset of the system */
int BT_Process(char *RxBuffer, int bytes, int *reset){
    int i = 0;
    char commands[COMMANDS_LEN] = COMMANDS;
    if (!strncmp(RxBuffer, "reset", 5)){                      // stop/reset program, go back to WAIT
        *reset = 1;
    } else {
        while (i < COMMANDS_LEN){
            while(UART_ReadTxStatus() & UART_TX_STS_FIFO_NOT_FULL)
                UART_PutChar(commands[i++]);
        }
        return 0;
    }
   
    while (i < bytes + 3){
        while(UART_ReadTxStatus() & UART_TX_STS_FIFO_NOT_FULL)
            UART_PutChar(RxBuffer[i++]);
    }
    return 0;
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
