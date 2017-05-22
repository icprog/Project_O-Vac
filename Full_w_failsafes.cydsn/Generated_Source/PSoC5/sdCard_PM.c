/*******************************************************************************
* File Name: sdCard_PM.c
* Version 1.20
*
* Description:
*  This file provides the API source code for Power Management of the emFile
*  component.
*
* Note:
*
*******************************************************************************
* Copyright 2011-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include <project.h>
#include "sdCard.h"


/*******************************************************************************
* Function Name: sdCard_SaveConfig
********************************************************************************
*
* Summary:
*  Saves SPI Masters configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void sdCard_SaveConfig(void) 
{
    #if(sdCard_NUMBER_SD_CARDS == 4u)
        sdCard_SPI0_SaveConfig();
        sdCard_SPI1_SaveConfig();
        sdCard_SPI2_SaveConfig();
        sdCard_SPI3_SaveConfig();
    #elif(sdCard_NUMBER_SD_CARDS == 3u)
        sdCard_SPI0_SaveConfig();
        sdCard_SPI1_SaveConfig();
        sdCard_SPI2_SaveConfig();
    #elif(sdCard_NUMBER_SD_CARDS == 2u)
        sdCard_SPI0_SaveConfig();
        sdCard_SPI1_SaveConfig();
    #else
        sdCard_SPI0_SaveConfig();
    #endif /* (sdCard_NUMBER_SD_CARDS == 4u) */
}


/*******************************************************************************
* Function Name: sdCard_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores SPI Masters configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  If this API is called without first calling SaveConfig then in the following
*  registers will be default values from Customizer:
*
*******************************************************************************/
void sdCard_RestoreConfig(void) 
{
    #if(sdCard_NUMBER_SD_CARDS == 4u)
        sdCard_SPI0_RestoreConfig();
        sdCard_SPI1_RestoreConfig();
        sdCard_SPI2_RestoreConfig();
        sdCard_SPI3_RestoreConfig();
    #elif(sdCard_NUMBER_SD_CARDS == 3u)
        sdCard_SPI0_SaveConfig();
        sdCard_SPI1_SaveConfig();
        sdCard_SPI2_SaveConfig();
    #elif(sdCard_NUMBER_SD_CARDS == 2u)
        sdCard_SPI0_SaveConfig();
        sdCard_SPI1_SaveConfig();
    #else
        sdCard_SPI0_SaveConfig();
    #endif /* (sdCard_NUMBER_SD_CARDS == 4u) */
}


/*******************************************************************************
* Function Name: sdCard_Sleep
********************************************************************************
*
* Summary:
*  Prepare emFile to go to sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void sdCard_Sleep(void) 
{
    #if(sdCard_NUMBER_SD_CARDS == 4u)
        sdCard_SPI0_Sleep();
        sdCard_SPI1_Sleep();
        sdCard_SPI2_Sleep();
        sdCard_SPI3_Sleep();
    #elif(sdCard_NUMBER_SD_CARDS == 3u)
        sdCard_SPI0_Sleep();
        sdCard_SPI1_Sleep();
        sdCard_SPI2_Sleep();
    #elif(sdCard_NUMBER_SD_CARDS == 2u)
        sdCard_SPI0_Sleep();
        sdCard_SPI1_Sleep();
    #else
        sdCard_SPI0_Sleep();
    #endif /* (sdCard_NUMBER_SD_CARDS == 4u) */
}


/*******************************************************************************
* Function Name: sdCard_Wakeup
********************************************************************************
*
* Summary:
*  Prepare SPIM Components to wake up.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void sdCard_Wakeup(void) 
{
    #if(sdCard_NUMBER_SD_CARDS == 4u)
        sdCard_SPI0_Wakeup();
        sdCard_SPI1_Wakeup();
        sdCard_SPI2_Wakeup();
        sdCard_SPI3_Wakeup();
    #elif(sdCard_NUMBER_SD_CARDS == 3u)
        sdCard_SPI0_Wakeup();
        sdCard_SPI1_Wakeup();
        sdCard_SPI2_Wakeup();
    #elif(sdCard_NUMBER_SD_CARDS == 2u)
        sdCard_SPI0_Wakeup();
        sdCard_SPI1_Wakeup();
    #else
        sdCard_SPI0_Wakeup();
    #endif /* (sdCard_NUMBER_SD_CARDS == 4u) */
}


/* [] END OF FILE */
