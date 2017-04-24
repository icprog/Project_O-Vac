/*******************************************************************************
* File Name: sdCard.h
* Version 1.20
*
* Description:
*  This file contains the function prototypes and constants used in the emFile
*  component.
*
* Note:
*
********************************************************************************
* Copyright 2011-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_EM_FILE_sdCard_H)
#define CY_EM_FILE_sdCard_H

#include <cytypes.h>
#include "cyfitter.h"
#include "MMC_X_HW.h"


/***************************************
*   Conditional Compilation Parameters
***************************************/

/* Number of configured SD cards */
#define sdCard_NUMBER_SD_CARDS    (1u)

/* Max frequency in KHz */
#define sdCard_MAX_SPI_FREQ       (4000u)

/* Enable Write Protect */
#define sdCard_WP0_EN             (0u)
#define sdCard_WP1_EN             (0u)
#define sdCard_WP2_EN             (0u)
#define sdCard_WP3_EN             (0u)


/***************************************
*        Function Prototypes
***************************************/

void sdCard_SaveConfig(void) ;
void sdCard_RestoreConfig(void) ;
void sdCard_Sleep(void) ;
void sdCard_Wakeup(void) ;


/***************************************
*           API Constants
***************************************/

#define sdCard_RET_SUCCCESS       (0x01u)
#define sdCard_RET_FAIL           (0x00u)

#endif /* CY_EM_FILE_sdCard_H */


/* [] END OF FILE */
