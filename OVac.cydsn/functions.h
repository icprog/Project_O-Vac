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
    
void I2C_LCD_print(uint8_t row, uint8_t column, uint16_t ax, uint16_t ay,uint16_t az);

int16_t ComputeMA(int16_t avg, int16_t n, int16_t sample);

char *ConvertToString(char *buf, char *string, ...);

#endif /* _FUNCTIONS_H_ */
/* [] END OF FILE */
