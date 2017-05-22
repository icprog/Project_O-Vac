/*******************************************************************************
* File Name: sdCard_MMC_HW_SPI.c
* Version 1.20
*
* Description:
*  Contains a set of File System APIs that implements SPI mode driver operation.
*
* Note:
*
********************************************************************************
* Copyright 2011-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "FS.h"
#include "sdCard.h"
#include "MMC_X_HW.h"
#include "project.h"


/*********************************************************************
*             Macros
*********************************************************************/
/* in mV, example means 3.3V */
#define sdCard_MMC_DEFAULT_SUPPLY_VOLTAGE    (3300u)

/* Max. startup frequency (KHz) */
#define sdCard_STARTUP_FREQ                   (400u)


/*********************************************************************
*       Static data
*********************************************************************/

static char sdCard_isInited0;

#if (sdCard_NUMBER_SD_CARDS >= 2u)

    static char sdCard_isInited1;

#endif /* (sdCard_NUMBER_SD_CARDS >= 2u) */

#if (sdCard_NUMBER_SD_CARDS >= 3u)

    static char sdCard_isInited2;

#endif /* (sdCard_NUMBER_SD_CARDS >= 3u) */

#if (sdCard_NUMBER_SD_CARDS == 4u)

    static char sdCard_isInited3;

#endif /* (sdCard_NUMBER_SD_CARDS == 4u) */


/*******************************************************************************
* Function Name: sdCard_Init
********************************************************************************
*
* Summary:
*  Initialize SPI Masters.
*
* Parameters:
*  Unit - Unit (SPIM) number.
*
* Return:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
static void sdCard_Init(U8 Unit) CYREENTRANT
{
    switch(Unit)
    {
    case 0u:

        if (sdCard_isInited0 == 0u)
        {
            /* Indicate that SPI 0 was initialized */
            sdCard_isInited0 = 1u;

            /* Initialize SPI */

            /* Stop the clock to set a required divider */
            sdCard_Clock_1_Stop();

            /* Set the inital clock frequency to 400 KHz */
            sdCard_Clock_1_SetDividerValue(BCLK__BUS_CLK__KHZ/sdCard_STARTUP_FREQ);

            /* Start the clock */
            sdCard_Clock_1_Start();

            /* Start SPI 0 */
            sdCard_SPI0_Start();
        }

        break;

        #if (sdCard_NUMBER_SD_CARDS >= 2u)

            case 1u:

                if (sdCard_isInited1 == 0u)
                {
                    /* Indicate that SPI 1 was initialized */
                    sdCard_isInited1 = 1u;

                    /* Stop the clock to set a required divider */
                    sdCard_Clock_2_Stop();

                    /* Set the inital clock frequency to 400 KHz */
                    sdCard_Clock_2_SetDividerValue(BCLK__BUS_CLK__KHZ/sdCard_STARTUP_FREQ);

                    /* Start the clock */
                    sdCard_Clock_2_Start();

                    /* Start SPI 1 */
                    sdCard_SPI1_Start();
                }

                break;

        #endif /* (sdCard_NUMBER_SD_CARDS >= 2u) */

        #if (sdCard_NUMBER_SD_CARDS >= 3u)

            case 2u:

                if (sdCard_isInited2 == 0u)
                {
                    /* Indicate that SPI 2 was initialized */
                    sdCard_isInited2 = 1u;

                    /* Stop the clock to set a required divider */
                    sdCard_Clock_3_Stop();

                    /* Set the inital clock frequency to 400 KHz */
                    sdCard_Clock_3_SetDividerValue(BCLK__BUS_CLK__KHZ/sdCard_STARTUP_FREQ);

                    /* Start the clock */
                    sdCard_Clock_3_Start();

                    /* Start SPI 2 */
                    sdCard_SPI2_Start();
                }

                break;

        #endif /* (sdCard_NUMBER_SD_CARDS >= 3u) */

        #if (sdCard_NUMBER_SD_CARDS == 4u)

            case 3u:

                if (sdCard_isInited3 == 0u)
                {
                    /* Indicate that SPI 3 was initialized */
                    sdCard_isInited3 = 1u;

                    /* Stop the clock to set a required divider */
                    sdCard_Clock_4_Stop();

                    /* Set the inital clock frequency to 400 KHz */
                    sdCard_Clock_4_SetDividerValue(BCLK__BUS_CLK__KHZ/sdCard_STARTUP_FREQ);

                    /* Start the clock */
                    sdCard_Clock_4_Start();

                    /* Start SPI 3 */
                    sdCard_SPI3_Start();
                }

                break;

        #endif /* (sdCard_NUMBER_SD_CARDS == 4u) */

    default:

        if (sdCard_isInited0 == 0u)
        {
            /* Indicate that SPI 0 was initialized */
            sdCard_isInited0 = 1u;

            /* Stop the clock to set a required divider */
            sdCard_Clock_1_Stop();

            /* Set the inital clock frequency to 400 KHz */
            sdCard_Clock_1_SetDividerValue(BCLK__BUS_CLK__KHZ/sdCard_STARTUP_FREQ);

            /* Start the clock */
            sdCard_Clock_1_Start();

            /* Start SPI 0 */
            sdCard_SPI0_Start();
        }
        
        break;
      }
}


/*******************************************************************************
* Function Name: sdCard_ReadWriteSPI
********************************************************************************
*
* Summary:
*  Reads and Writes data via SPI.
*
* Parameters:
*  Unit - Unit number;
*  Data - data to be written.
*
* Return:
*  Data received from SD card.
*
* Reentrant:
*  No
*
*******************************************************************************/
static U8 sdCard_ReadWriteSPI(U8 Unit, U8 Data) CYREENTRANT
{
    U8 spiData;

    switch(Unit)
    {
    case 0u:

        /* Send Data */
        sdCard_SPI0_WriteTxData(Data);

        /* Wait until all bits are shifted */
        while (0u == (sdCard_SPI0_STS_SPI_DONE & sdCard_SPI0_ReadTxStatus()));

        /* Read data */
        spiData = sdCard_SPI0_ReadRxData();

        break;

    #if (sdCard_NUMBER_SD_CARDS >= 2u)

        case 1u:

            /* Send Data */
            sdCard_SPI1_WriteTxData(Data);

            /* Wait until all bits are shifted */
            while (0u == (sdCard_SPI1_STS_SPI_DONE & sdCard_SPI1_ReadTxStatus()));

            /* Read data */
            spiData = sdCard_SPI1_ReadRxData();

            break;

    #endif /* (sdCard_NUMBER_SD_CARDS >= 2u) */

    #if (sdCard_NUMBER_SD_CARDS >= 3u)

        case 2u:

            /* Send Data */
            sdCard_SPI2_WriteTxData(Data);

            /* Wait until all bits are shifted */
            while (0u == (sdCard_SPI2_STS_SPI_DONE & sdCard_SPI2_ReadTxStatus()));

            /* Read data */
            spiData = sdCard_SPI2_ReadRxData();

            break;

    #endif /* (sdCard_NUMBER_SD_CARDS >= 2u) */

    #if (sdCard_NUMBER_SD_CARDS >= 4u)

        case 3u:

            /* Send Data */
            sdCard_SPI3_WriteTxData(Data);

            /* Wait until all bits are shifted */
            while (0u == (sdCard_SPI3_STS_SPI_DONE & sdCard_SPI3_ReadTxStatus()));

            /* Read data */
            spiData = sdCard_SPI3_ReadRxData();

            break;

    #endif /* (sdCard_NUMBER_SD_CARDS >= 4u) */

    default:

        /* Send Data */
        sdCard_SPI0_WriteTxData(Data);

        /* Wait until all bits are shifted */
        while (0u == (sdCard_SPI0_STS_SPI_DONE & sdCard_SPI0_ReadTxStatus()));

        /* Read data */
        spiData = sdCard_SPI0_ReadRxData();
        
        break;
    }

    return(spiData);
}

/*******************************************************************************
* Function Name: FS_MMC_HW_X_EnableCS
********************************************************************************
*
* Summary:
*  FS low level function. Sets the card slot active using the
*  chip select (CS) line.
*
* Parameters:
*  Unit      - Device Index.
*
* Return:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void FS_MMC_HW_X_EnableCS(U8 Unit)
{
    switch(Unit)
    {
        case 0u:

            /* Set CS to 0 */
            sdCard_SPI0_CS_Write(0u);

            break;

        #if (sdCard_NUMBER_SD_CARDS >= 2u)

            case 1u:

                /* Set CS to 0 */
                sdCard_SPI1_CS_Write(0u);

                break;

        #endif /* (sdCard_NUMBER_SD_CARDS >= 2u) */

        #if (sdCard_NUMBER_SD_CARDS >= 3u)

            case 2u:

                /* Set CS to 0 */
                sdCard_SPI2_CS_Write(0u);

                break;

        #endif /* (sdCard_NUMBER_SD_CARDS >= 3u) */

        #if (sdCard_NUMBER_SD_CARDS == 4u)

            case 3u:

                /* Set CS to 0 */
                sdCard_SPI3_CS_Write(0u);

                break;

        #endif /* (sdCard_NUMBER_SD_CARDS == 4u) */

        default:

            sdCard_SPI0_CS_Write(0u);

            break;
    }
}


/*******************************************************************************
* Function Name: FS_MMC_HW_X_DisableCS
********************************************************************************
*
* Summary:
*  FS low level function. Clears the card slot inactive using the
*  chip select (CS) line.
*
* Parameters:
*  Unit      - Device Index.
*
* Return:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
void FS_MMC_HW_X_DisableCS(U8 Unit)
{
    switch(Unit)
    {
    case 0u:

        sdCard_SPI0_CS_Write(1u);

        break;

    #if (sdCard_NUMBER_SD_CARDS >= 2u)

        case 1u:

            sdCard_SPI1_CS_Write(1u);

            break;

    #endif /* (sdCard_NUMBER_SD_CARDS >= 2u) */

    #if (sdCard_NUMBER_SD_CARDS >= 3u)

        case 2u:

            sdCard_SPI2_CS_Write(1u);

            break;

    #endif /* (sdCard_NUMBER_SD_CARDS >= 3u) */

    #if (sdCard_NUMBER_SD_CARDS == 4u)

        case 3u:

            sdCard_SPI3_CS_Write(1u);

            break;

    #endif /* (sdCard_NUMBER_SD_CARDS == 4u) */

    default:

        sdCard_SPI0_CS_Write(1u);
        
        break;
    }
}


/*******************************************************************************
* Function Name: FS_MMC_HW_X_IsWriteProtected
********************************************************************************
*
* Summary:
*  FS low level function. Returns the state of the physical write
*  protection of the SD cards.
*
* Parameters:
*  Unit      - Device Index.
*
* Return:
*    1       - the card is write protected;
*    0       - the card is not write protected.
*
* Reentrant:
*  No
*
*******************************************************************************/
int FS_MMC_HW_X_IsWriteProtected(U8 Unit)
{
   int wpState;

    switch(Unit)
    {
    case 0u:

        #if (sdCard_WP0_EN)

            /* Based on physical switch state */
              wpState = (int)sdCard_SPI0_WP_Read();

        #else

            wpState = 0;

        #endif /* (sdCard_WP0_EN) */

        break;

    #if (sdCard_NUMBER_SD_CARDS >= 2u)

        case 1u:

            #if (sdCard_WP1_EN)

                   /* Based on physical switch state */
                wpState = (int)sdCard_SPI1_WP_Read();

            #else

                wpState = 0;

            #endif /* (sdCard_WP1_EN) */

            break;

    #endif /* (sdCard_NUMBER_SD_CARDS >= 2u) */

    #if (sdCard_NUMBER_SD_CARDS >= 3u)

        case 2u:

            #if (sdCard_WP2_EN)

                  /* Based on physical switch state */
                wpState = (int)sdCard_SPI2_WP_Read();

            #else

                wpState = 0;

            #endif /* (sdCard_WP2_EN) */

            break;

    #endif /* (sdCard_NUMBER_SD_CARDS >= 3u) */

    #if (sdCard_NUMBER_SD_CARDS == 4u)

        case 3u:

            #if (sdCard_WP3_EN)

                /* Based on physical switch state */
                wpState = (int)sdCard_SPI3_WP_Read();

            #else

                wpState = 0;

            #endif /* (sdCard_WP3_EN) */

            break;

    #endif /* (sdCard_NUMBER_SD_CARDS == 4u) */

    default:

        #if (sdCard_WP0_EN)

              /* Based on physical switch state */
            wpState = (int)sdCard_SPI0_WP_Read();

        #else

            wpState = 0;

        #endif /* (sdCard_WP0_EN) */
        
        break;
    }

    return(wpState);
}


/*******************************************************************************
* Function Name: FS_MMC_HW_X_SetMaxSpeed
********************************************************************************
*
* Summary:
*  FS low level function. Sets the SPI interface to a maximum frequency.
*  Make sure that you set the frequency lower or equal but never higher
*  than the given value. Recommended startup frequency is 100kHz - 400kHz.
*
* Parameters:
*  Unit             - Device Index;
*  MaxFreq          - SPI clock frequency in kHz.
*
* Return:
*  max. frequency   - the maximum frequency set in kHz;
*  0                - the frequency could not be set.
*
* Reentrant:
*  No
*
*******************************************************************************/
U16 FS_MMC_HW_X_SetMaxSpeed(U8 Unit, U16 MaxFreq)
{
    U16 freq;
    U32 divResult;

    if(MaxFreq < sdCard_STARTUP_FREQ)
    {
        MaxFreq = sdCard_STARTUP_FREQ;
    }
    else if(MaxFreq > sdCard_MAX_SPI_FREQ)
    {
        MaxFreq = sdCard_MAX_SPI_FREQ;
    }
    else
    {
        /* Do nothing */
    }

    freq = MaxFreq << 1u;

    divResult = ((U32) BCLK__BUS_CLK__KHZ)/((U32) freq);

    switch(Unit)
    {
        case 0u:
            sdCard_Clock_1_Stop();
            sdCard_Clock_1_SetDividerValue((U16)divResult);    /* update the frequency */
            sdCard_Clock_1_Start();
            break;

        #if (sdCard_NUMBER_SD_CARDS >= 2u)

        case 1u:

            sdCard_Clock_2_Stop();
            sdCard_Clock_2_SetDividerValue((U16)divResult);    /* update the frequency */
            sdCard_Clock_2_Start();
            break;

        #endif /* (sdCard_NUMBER_SD_CARDS >= 2u) */

        #if (sdCard_NUMBER_SD_CARDS >= 3u)

        case 2u:

            sdCard_Clock_3_Stop();
            sdCard_Clock_3_SetDividerValue((U16)divResult);    /* update the frequency */
            sdCard_Clock_3_Start();
            break;

        #endif /* (sdCard_NUMBER_SD_CARDS >= 3u) */

        #if (sdCard_NUMBER_SD_CARDS == 4u)

        case 3u:

            sdCard_Clock_4_Stop();
            sdCard_Clock_4_SetDividerValue((U16)divResult);    /* update the frequency */
            sdCard_Clock_4_Start();
            break;

        #endif /* (sdCard_NUMBER_SD_CARDS == 4u) */

        default:
            sdCard_Clock_1_Stop();
            sdCard_Clock_1_SetDividerValue((U16)divResult);    /* update the frequency */
            sdCard_Clock_1_Start();
            break;
    }

    return (MaxFreq);

}


/*******************************************************************************
* Function Name: FS_MMC_HW_X_SetVoltage
********************************************************************************
*
* Summary:
*  FS low level function. Be sure that your card slot si within the given
*  voltage range. Return 1 if your slot can support the required voltage,
*  and if not, return 0.
*
* Parameters:
*  Unit             - Device Index;
*  Vmin             - Minimum supply voltage in mV;
*  Vmin             - Maximum supply voltage in mV.
*
* Return:
*  1                - the card slot supports the voltage range;
*  0                - the card slot does not support the voltage range.
*
* Reentrant:
*  No
*
*******************************************************************************/
int FS_MMC_HW_X_SetVoltage(U8 Unit, U16 Vmin, U16 Vmax)
{
    int result;

    switch(Unit)
    {
    case 0u:

    case 1u:

    case 2u:

    case 3u:

        if((Vmin <= sdCard_MMC_DEFAULT_SUPPLY_VOLTAGE) &&
            (sdCard_MMC_DEFAULT_SUPPLY_VOLTAGE <= Vmax))
        {
            result = sdCard_RET_SUCCCESS;
        }
        else
        {
            result = sdCard_RET_FAIL;
        }

        break;

    default:

        if((Vmin <= sdCard_MMC_DEFAULT_SUPPLY_VOLTAGE) &&
            (sdCard_MMC_DEFAULT_SUPPLY_VOLTAGE <= Vmax))
        {
            result = sdCard_RET_SUCCCESS;
        }
        else
        {
            result = sdCard_RET_FAIL;
        }
        
        break;
    }

    return(result);
}


/*******************************************************************************
* Function Name: FS_MMC_HW_X_IsPresent
********************************************************************************
*
* Summary:
*  Returns the state of the media. If you do not know the state, return
*  FS_MEDIA_STATE_UNKNOWN and the higher layer will try to figure out if
*  a media is present.
*
* Parameters:
*  Unit                      - Device Index.
*
* Return:
*  FS_MEDIA_STATE_UNKNOWN    - Media state is unknown;
*  FS_MEDIA_NOT_PRESENT      - Media is not present;
*  FS_MEDIA_IS_PRESENT       - Media is present.
*
* Reentrant:
*  No
*
*******************************************************************************/
int FS_MMC_HW_X_IsPresent(U8 Unit)
{
     int result;

    sdCard_Init(Unit);

    switch(Unit)
    {
    case 0u:

    case 1u:

    case 2u:

    case 3u:

        result = FS_MEDIA_STATE_UNKNOWN;
        break;

    default:

        result = FS_MEDIA_STATE_UNKNOWN;
        break;
    }

    return(result);
}


/*******************************************************************************
* Function Name: FS_MMC_HW_X_Read
********************************************************************************
*
* Summary:
*  FS low level function. Reads a specified number of bytes from MMC card to 
*  buffer.
*
* Parameters:
*    Unit             - Device Index;
*    pData            - Pointer to a data buffer;
*    NumBytes         - Number of bytes.
*
* Return:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
#if (CY_PSOC5)
    void  FS_MMC_HW_X_Read (U8 Unit, U8 * pData, int NumBytes)
    {
        do
        {
            *pData++ = sdCard_ReadWriteSPI(Unit, 0xff);
        } while (--NumBytes);
    }
#else
    void  FS_MMC_HW_X_Read (U8 Unit, U8 xdata * pData, int NumBytes)
    {
        do
        {
            *pData++ = sdCard_ReadWriteSPI(Unit, 0xff);
        } while (--NumBytes);
    }
#endif /* (CY_PSOC5) */


/*******************************************************************************
* Function Name: FS_MMC_HW_X_Write
********************************************************************************
*
* Summary:
*  FS low level function. Writes a specified number of bytes from
*  data buffer to the MMC/SD card.
*
* Parameters:
*    Unit             - Device Index;
*    pData            - Pointer to a data buffer;
*    NumBytes         - Number of bytes.
*
* Return:
*  None
*
* Reentrant:
*  No
*
*******************************************************************************/
#if (CY_PSOC5)
    void  FS_MMC_HW_X_Write(U8 Unit, const U8 * pData, int NumBytes) 
    {
        do
        {
            sdCard_ReadWriteSPI(Unit, *pData++);
        } while (--NumBytes);
    }
#else
    void  FS_MMC_HW_X_Write(U8 Unit, const U8 xdata * pData, int NumBytes) 
    {
        do
        {
            sdCard_ReadWriteSPI(Unit, *pData++);
        } while (--NumBytes);
    }
#endif  /* (CY_PSOC5) */


/* [] END OF FILE */
