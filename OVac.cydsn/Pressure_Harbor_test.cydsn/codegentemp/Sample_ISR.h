/*******************************************************************************
* File Name: Sample_ISR.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_Sample_ISR_H)
#define CY_ISR_Sample_ISR_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void Sample_ISR_Start(void);
void Sample_ISR_StartEx(cyisraddress address);
void Sample_ISR_Stop(void);

CY_ISR_PROTO(Sample_ISR_Interrupt);

void Sample_ISR_SetVector(cyisraddress address);
cyisraddress Sample_ISR_GetVector(void);

void Sample_ISR_SetPriority(uint8 priority);
uint8 Sample_ISR_GetPriority(void);

void Sample_ISR_Enable(void);
uint8 Sample_ISR_GetState(void);
void Sample_ISR_Disable(void);

void Sample_ISR_SetPending(void);
void Sample_ISR_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the Sample_ISR ISR. */
#define Sample_ISR_INTC_VECTOR            ((reg32 *) Sample_ISR__INTC_VECT)

/* Address of the Sample_ISR ISR priority. */
#define Sample_ISR_INTC_PRIOR             ((reg8 *) Sample_ISR__INTC_PRIOR_REG)

/* Priority of the Sample_ISR interrupt. */
#define Sample_ISR_INTC_PRIOR_NUMBER      Sample_ISR__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable Sample_ISR interrupt. */
#define Sample_ISR_INTC_SET_EN            ((reg32 *) Sample_ISR__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the Sample_ISR interrupt. */
#define Sample_ISR_INTC_CLR_EN            ((reg32 *) Sample_ISR__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the Sample_ISR interrupt state to pending. */
#define Sample_ISR_INTC_SET_PD            ((reg32 *) Sample_ISR__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the Sample_ISR interrupt. */
#define Sample_ISR_INTC_CLR_PD            ((reg32 *) Sample_ISR__INTC_CLR_PD_REG)


#endif /* CY_ISR_Sample_ISR_H */


/* [] END OF FILE */
