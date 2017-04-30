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
#include <FS.h>
#include <stdbool.h>

#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_
    
#define COMMANDS "\ncommands: \n05,start\n" \
                 "04,stop\n" \
                 "04,data\n"

#define COMMANDS_LEN 37
    
/*State Declarations*/
typedef enum STATES{
    SYSTEM_CHECK, 
    WAIT_TO_LAUNCH,
    DESCENDING,
    LANDED,
    RESURFACE,
    TRANSMIT,
    ERROR
}STATES;      
    
void I2C_LCD_print(uint8_t row, uint8_t column, uint16_t ax, uint16_t ay,uint16_t az);

float ComputeMA(float avg, int16_t n, float sample);

void BT_Process(char *RxBuffer, STATES *STATE, int bytes, int *dataflag);

void BT_Send(char *RxBuffer, STATES *STATE, int lengthOfBuf, int *firstPacket);

void uint8_to_char(uint8_t a[], char b[], int len);

#endif /* _FUNCTIONS_H_ */
/* [] END OF FILE */
