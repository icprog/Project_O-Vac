/*******************************************************************************
* File Name: sdCard_SPI0_CS.h  
* Version 2.0
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_sdCard_SPI0_CS_H) /* Pins sdCard_SPI0_CS_H */
#define CY_PINS_sdCard_SPI0_CS_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "sdCard_SPI0_CS_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_0 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 sdCard_SPI0_CS__PORT == 15 && ((sdCard_SPI0_CS__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    sdCard_SPI0_CS_Write(uint8 value) ;
void    sdCard_SPI0_CS_SetDriveMode(uint8 mode) ;
uint8   sdCard_SPI0_CS_ReadDataReg(void) ;
uint8   sdCard_SPI0_CS_Read(void) ;
uint8   sdCard_SPI0_CS_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define sdCard_SPI0_CS_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define sdCard_SPI0_CS_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define sdCard_SPI0_CS_DM_RES_UP          PIN_DM_RES_UP
#define sdCard_SPI0_CS_DM_RES_DWN         PIN_DM_RES_DWN
#define sdCard_SPI0_CS_DM_OD_LO           PIN_DM_OD_LO
#define sdCard_SPI0_CS_DM_OD_HI           PIN_DM_OD_HI
#define sdCard_SPI0_CS_DM_STRONG          PIN_DM_STRONG
#define sdCard_SPI0_CS_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define sdCard_SPI0_CS_MASK               sdCard_SPI0_CS__MASK
#define sdCard_SPI0_CS_SHIFT              sdCard_SPI0_CS__SHIFT
#define sdCard_SPI0_CS_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define sdCard_SPI0_CS_PS                     (* (reg8 *) sdCard_SPI0_CS__PS)
/* Data Register */
#define sdCard_SPI0_CS_DR                     (* (reg8 *) sdCard_SPI0_CS__DR)
/* Port Number */
#define sdCard_SPI0_CS_PRT_NUM                (* (reg8 *) sdCard_SPI0_CS__PRT) 
/* Connect to Analog Globals */                                                  
#define sdCard_SPI0_CS_AG                     (* (reg8 *) sdCard_SPI0_CS__AG)                       
/* Analog MUX bux enable */
#define sdCard_SPI0_CS_AMUX                   (* (reg8 *) sdCard_SPI0_CS__AMUX) 
/* Bidirectional Enable */                                                        
#define sdCard_SPI0_CS_BIE                    (* (reg8 *) sdCard_SPI0_CS__BIE)
/* Bit-mask for Aliased Register Access */
#define sdCard_SPI0_CS_BIT_MASK               (* (reg8 *) sdCard_SPI0_CS__BIT_MASK)
/* Bypass Enable */
#define sdCard_SPI0_CS_BYP                    (* (reg8 *) sdCard_SPI0_CS__BYP)
/* Port wide control signals */                                                   
#define sdCard_SPI0_CS_CTL                    (* (reg8 *) sdCard_SPI0_CS__CTL)
/* Drive Modes */
#define sdCard_SPI0_CS_DM0                    (* (reg8 *) sdCard_SPI0_CS__DM0) 
#define sdCard_SPI0_CS_DM1                    (* (reg8 *) sdCard_SPI0_CS__DM1)
#define sdCard_SPI0_CS_DM2                    (* (reg8 *) sdCard_SPI0_CS__DM2) 
/* Input Buffer Disable Override */
#define sdCard_SPI0_CS_INP_DIS                (* (reg8 *) sdCard_SPI0_CS__INP_DIS)
/* LCD Common or Segment Drive */
#define sdCard_SPI0_CS_LCD_COM_SEG            (* (reg8 *) sdCard_SPI0_CS__LCD_COM_SEG)
/* Enable Segment LCD */
#define sdCard_SPI0_CS_LCD_EN                 (* (reg8 *) sdCard_SPI0_CS__LCD_EN)
/* Slew Rate Control */
#define sdCard_SPI0_CS_SLW                    (* (reg8 *) sdCard_SPI0_CS__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define sdCard_SPI0_CS_PRTDSI__CAPS_SEL       (* (reg8 *) sdCard_SPI0_CS__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define sdCard_SPI0_CS_PRTDSI__DBL_SYNC_IN    (* (reg8 *) sdCard_SPI0_CS__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define sdCard_SPI0_CS_PRTDSI__OE_SEL0        (* (reg8 *) sdCard_SPI0_CS__PRTDSI__OE_SEL0) 
#define sdCard_SPI0_CS_PRTDSI__OE_SEL1        (* (reg8 *) sdCard_SPI0_CS__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define sdCard_SPI0_CS_PRTDSI__OUT_SEL0       (* (reg8 *) sdCard_SPI0_CS__PRTDSI__OUT_SEL0) 
#define sdCard_SPI0_CS_PRTDSI__OUT_SEL1       (* (reg8 *) sdCard_SPI0_CS__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define sdCard_SPI0_CS_PRTDSI__SYNC_OUT       (* (reg8 *) sdCard_SPI0_CS__PRTDSI__SYNC_OUT) 


#if defined(sdCard_SPI0_CS__INTSTAT)  /* Interrupt Registers */

    #define sdCard_SPI0_CS_INTSTAT                (* (reg8 *) sdCard_SPI0_CS__INTSTAT)
    #define sdCard_SPI0_CS_SNAP                   (* (reg8 *) sdCard_SPI0_CS__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_sdCard_SPI0_CS_H */


/* [] END OF FILE */
