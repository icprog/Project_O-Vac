/*******************************************************************************
* File Name: Sample_Timer_PM.c
* Version 2.70
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "Sample_Timer.h"

static Sample_Timer_backupStruct Sample_Timer_backup;


/*******************************************************************************
* Function Name: Sample_Timer_SaveConfig
********************************************************************************
*
* Summary:
*     Save the current user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Sample_Timer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Sample_Timer_SaveConfig(void) 
{
    #if (!Sample_Timer_UsingFixedFunction)
        Sample_Timer_backup.TimerUdb = Sample_Timer_ReadCounter();
        Sample_Timer_backup.InterruptMaskValue = Sample_Timer_STATUS_MASK;
        #if (Sample_Timer_UsingHWCaptureCounter)
            Sample_Timer_backup.TimerCaptureCounter = Sample_Timer_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!Sample_Timer_UDB_CONTROL_REG_REMOVED)
            Sample_Timer_backup.TimerControlRegister = Sample_Timer_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: Sample_Timer_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Sample_Timer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Sample_Timer_RestoreConfig(void) 
{   
    #if (!Sample_Timer_UsingFixedFunction)

        Sample_Timer_WriteCounter(Sample_Timer_backup.TimerUdb);
        Sample_Timer_STATUS_MASK =Sample_Timer_backup.InterruptMaskValue;
        #if (Sample_Timer_UsingHWCaptureCounter)
            Sample_Timer_SetCaptureCount(Sample_Timer_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!Sample_Timer_UDB_CONTROL_REG_REMOVED)
            Sample_Timer_WriteControlRegister(Sample_Timer_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: Sample_Timer_Sleep
********************************************************************************
*
* Summary:
*     Stop and Save the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Sample_Timer_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void Sample_Timer_Sleep(void) 
{
    #if(!Sample_Timer_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(Sample_Timer_CTRL_ENABLE == (Sample_Timer_CONTROL & Sample_Timer_CTRL_ENABLE))
        {
            /* Timer is enabled */
            Sample_Timer_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            Sample_Timer_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    Sample_Timer_Stop();
    Sample_Timer_SaveConfig();
}


/*******************************************************************************
* Function Name: Sample_Timer_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Sample_Timer_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Sample_Timer_Wakeup(void) 
{
    Sample_Timer_RestoreConfig();
    #if(!Sample_Timer_UDB_CONTROL_REG_REMOVED)
        if(Sample_Timer_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                Sample_Timer_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
