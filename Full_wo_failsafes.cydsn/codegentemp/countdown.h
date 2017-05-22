/*******************************************************************************
* File Name: countdown.h
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
#if !defined(CY_ISR_countdown_H)
#define CY_ISR_countdown_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void countdown_Start(void);
void countdown_StartEx(cyisraddress address);
void countdown_Stop(void);

CY_ISR_PROTO(countdown_Interrupt);

void countdown_SetVector(cyisraddress address);
cyisraddress countdown_GetVector(void);

void countdown_SetPriority(uint8 priority);
uint8 countdown_GetPriority(void);

void countdown_Enable(void);
uint8 countdown_GetState(void);
void countdown_Disable(void);

void countdown_SetPending(void);
void countdown_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the countdown ISR. */
#define countdown_INTC_VECTOR            ((reg32 *) countdown__INTC_VECT)

/* Address of the countdown ISR priority. */
#define countdown_INTC_PRIOR             ((reg8 *) countdown__INTC_PRIOR_REG)

/* Priority of the countdown interrupt. */
#define countdown_INTC_PRIOR_NUMBER      countdown__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable countdown interrupt. */
#define countdown_INTC_SET_EN            ((reg32 *) countdown__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the countdown interrupt. */
#define countdown_INTC_CLR_EN            ((reg32 *) countdown__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the countdown interrupt state to pending. */
#define countdown_INTC_SET_PD            ((reg32 *) countdown__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the countdown interrupt. */
#define countdown_INTC_CLR_PD            ((reg32 *) countdown__INTC_CLR_PD_REG)


#endif /* CY_ISR_countdown_H */


/* [] END OF FILE */
