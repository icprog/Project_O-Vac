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

int16_t ComputeMA(int16_t avg, int16_t n, int16_t sample);

void Process_BT(uint8_t *RxBuffer, STATES *STATE, int bytes);

#endif /* _FUNCTIONS_H_ */
/* [] END OF FILE */
