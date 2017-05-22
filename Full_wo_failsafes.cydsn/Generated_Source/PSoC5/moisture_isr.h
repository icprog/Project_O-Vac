/*******************************************************************************
* File Name: moisture_isr.h
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
#if !defined(CY_ISR_moisture_isr_H)
#define CY_ISR_moisture_isr_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void moisture_isr_Start(void);
void moisture_isr_StartEx(cyisraddress address);
void moisture_isr_Stop(void);

CY_ISR_PROTO(moisture_isr_Interrupt);

void moisture_isr_SetVector(cyisraddress address);
cyisraddress moisture_isr_GetVector(void);

void moisture_isr_SetPriority(uint8 priority);
uint8 moisture_isr_GetPriority(void);

void moisture_isr_Enable(void);
uint8 moisture_isr_GetState(void);
void moisture_isr_Disable(void);

void moisture_isr_SetPending(void);
void moisture_isr_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the moisture_isr ISR. */
#define moisture_isr_INTC_VECTOR            ((reg32 *) moisture_isr__INTC_VECT)

/* Address of the moisture_isr ISR priority. */
#define moisture_isr_INTC_PRIOR             ((reg8 *) moisture_isr__INTC_PRIOR_REG)

/* Priority of the moisture_isr interrupt. */
#define moisture_isr_INTC_PRIOR_NUMBER      moisture_isr__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable moisture_isr interrupt. */
#define moisture_isr_INTC_SET_EN            ((reg32 *) moisture_isr__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the moisture_isr interrupt. */
#define moisture_isr_INTC_CLR_EN            ((reg32 *) moisture_isr__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the moisture_isr interrupt state to pending. */
#define moisture_isr_INTC_SET_PD            ((reg32 *) moisture_isr__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the moisture_isr interrupt. */
#define moisture_isr_INTC_CLR_PD            ((reg32 *) moisture_isr__INTC_CLR_PD_REG)


#endif /* CY_ISR_moisture_isr_H */


/* [] END OF FILE */
