/*******************************************************************************
* File Name: sdCard_SPI0.h
* Version 2.40
*
* Description:
*  Contains the function prototypes, constants and register definition
*  of the SPI Master Component.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SPIM_sdCard_SPI0_H)
#define CY_SPIM_sdCard_SPI0_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component SPI_Master_v2_40 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */


/***************************************
*   Conditional Compilation Parameters
***************************************/

#define sdCard_SPI0_INTERNAL_CLOCK             (0u)

#if(0u != sdCard_SPI0_INTERNAL_CLOCK)
    #include "sdCard_SPI0_IntClock.h"
#endif /* (0u != sdCard_SPI0_INTERNAL_CLOCK) */

#define sdCard_SPI0_MODE                       (1u)
#define sdCard_SPI0_DATA_WIDTH                 (8u)
#define sdCard_SPI0_MODE_USE_ZERO              (1u)
#define sdCard_SPI0_BIDIRECTIONAL_MODE         (0u)

/* Internal interrupt handling */
#define sdCard_SPI0_TX_BUFFER_SIZE             (4u)
#define sdCard_SPI0_RX_BUFFER_SIZE             (4u)
#define sdCard_SPI0_INTERNAL_TX_INT_ENABLED    (0u)
#define sdCard_SPI0_INTERNAL_RX_INT_ENABLED    (0u)

#define sdCard_SPI0_SINGLE_REG_SIZE            (8u)
#define sdCard_SPI0_USE_SECOND_DATAPATH        (sdCard_SPI0_DATA_WIDTH > sdCard_SPI0_SINGLE_REG_SIZE)

#define sdCard_SPI0_FIFO_SIZE                  (4u)
#define sdCard_SPI0_TX_SOFTWARE_BUF_ENABLED    ((0u != sdCard_SPI0_INTERNAL_TX_INT_ENABLED) && \
                                                     (sdCard_SPI0_TX_BUFFER_SIZE > sdCard_SPI0_FIFO_SIZE))

#define sdCard_SPI0_RX_SOFTWARE_BUF_ENABLED    ((0u != sdCard_SPI0_INTERNAL_RX_INT_ENABLED) && \
                                                     (sdCard_SPI0_RX_BUFFER_SIZE > sdCard_SPI0_FIFO_SIZE))


/***************************************
*        Data Struct Definition
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 enableState;
    uint8 cntrPeriod;
    #if(CY_UDB_V0)
        uint8 saveSrTxIntMask;
        uint8 saveSrRxIntMask;
    #endif /* (CY_UDB_V0) */

} sdCard_SPI0_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

void  sdCard_SPI0_Init(void)                           ;
void  sdCard_SPI0_Enable(void)                         ;
void  sdCard_SPI0_Start(void)                          ;
void  sdCard_SPI0_Stop(void)                           ;

void  sdCard_SPI0_EnableTxInt(void)                    ;
void  sdCard_SPI0_EnableRxInt(void)                    ;
void  sdCard_SPI0_DisableTxInt(void)                   ;
void  sdCard_SPI0_DisableRxInt(void)                   ;

void  sdCard_SPI0_Sleep(void)                          ;
void  sdCard_SPI0_Wakeup(void)                         ;
void  sdCard_SPI0_SaveConfig(void)                     ;
void  sdCard_SPI0_RestoreConfig(void)                  ;

void  sdCard_SPI0_SetTxInterruptMode(uint8 intSrc)     ;
void  sdCard_SPI0_SetRxInterruptMode(uint8 intSrc)     ;
uint8 sdCard_SPI0_ReadTxStatus(void)                   ;
uint8 sdCard_SPI0_ReadRxStatus(void)                   ;
void  sdCard_SPI0_WriteTxData(uint8 txData)  \
                                                            ;
uint8 sdCard_SPI0_ReadRxData(void) \
                                                            ;
uint8 sdCard_SPI0_GetRxBufferSize(void)                ;
uint8 sdCard_SPI0_GetTxBufferSize(void)                ;
void  sdCard_SPI0_ClearRxBuffer(void)                  ;
void  sdCard_SPI0_ClearTxBuffer(void)                  ;
void  sdCard_SPI0_ClearFIFO(void)                              ;
void  sdCard_SPI0_PutArray(const uint8 buffer[], uint8 byteCount) \
                                                            ;

#if(0u != sdCard_SPI0_BIDIRECTIONAL_MODE)
    void  sdCard_SPI0_TxEnable(void)                   ;
    void  sdCard_SPI0_TxDisable(void)                  ;
#endif /* (0u != sdCard_SPI0_BIDIRECTIONAL_MODE) */

CY_ISR_PROTO(sdCard_SPI0_TX_ISR);
CY_ISR_PROTO(sdCard_SPI0_RX_ISR);


/**********************************
*   Variable with external linkage
**********************************/

extern uint8 sdCard_SPI0_initVar;


/***************************************
*           API Constants
***************************************/

#define sdCard_SPI0_TX_ISR_NUMBER     ((uint8) (sdCard_SPI0_TxInternalInterrupt__INTC_NUMBER))
#define sdCard_SPI0_RX_ISR_NUMBER     ((uint8) (sdCard_SPI0_RxInternalInterrupt__INTC_NUMBER))

#define sdCard_SPI0_TX_ISR_PRIORITY   ((uint8) (sdCard_SPI0_TxInternalInterrupt__INTC_PRIOR_NUM))
#define sdCard_SPI0_RX_ISR_PRIORITY   ((uint8) (sdCard_SPI0_RxInternalInterrupt__INTC_PRIOR_NUM))


/***************************************
*    Initial Parameter Constants
***************************************/

#define sdCard_SPI0_INT_ON_SPI_DONE    ((uint8) (0u   << sdCard_SPI0_STS_SPI_DONE_SHIFT))
#define sdCard_SPI0_INT_ON_TX_EMPTY    ((uint8) (0u   << sdCard_SPI0_STS_TX_FIFO_EMPTY_SHIFT))
#define sdCard_SPI0_INT_ON_TX_NOT_FULL ((uint8) (0u << \
                                                                           sdCard_SPI0_STS_TX_FIFO_NOT_FULL_SHIFT))
#define sdCard_SPI0_INT_ON_BYTE_COMP   ((uint8) (0u  << sdCard_SPI0_STS_BYTE_COMPLETE_SHIFT))
#define sdCard_SPI0_INT_ON_SPI_IDLE    ((uint8) (0u   << sdCard_SPI0_STS_SPI_IDLE_SHIFT))

/* Disable TX_NOT_FULL if software buffer is used */
#define sdCard_SPI0_INT_ON_TX_NOT_FULL_DEF ((sdCard_SPI0_TX_SOFTWARE_BUF_ENABLED) ? \
                                                                        (0u) : (sdCard_SPI0_INT_ON_TX_NOT_FULL))

/* TX interrupt mask */
#define sdCard_SPI0_TX_INIT_INTERRUPTS_MASK    (sdCard_SPI0_INT_ON_SPI_DONE  | \
                                                     sdCard_SPI0_INT_ON_TX_EMPTY  | \
                                                     sdCard_SPI0_INT_ON_TX_NOT_FULL_DEF | \
                                                     sdCard_SPI0_INT_ON_BYTE_COMP | \
                                                     sdCard_SPI0_INT_ON_SPI_IDLE)

#define sdCard_SPI0_INT_ON_RX_FULL         ((uint8) (0u << \
                                                                          sdCard_SPI0_STS_RX_FIFO_FULL_SHIFT))
#define sdCard_SPI0_INT_ON_RX_NOT_EMPTY    ((uint8) (0u << \
                                                                          sdCard_SPI0_STS_RX_FIFO_NOT_EMPTY_SHIFT))
#define sdCard_SPI0_INT_ON_RX_OVER         ((uint8) (0u << \
                                                                          sdCard_SPI0_STS_RX_FIFO_OVERRUN_SHIFT))

/* RX interrupt mask */
#define sdCard_SPI0_RX_INIT_INTERRUPTS_MASK    (sdCard_SPI0_INT_ON_RX_FULL      | \
                                                     sdCard_SPI0_INT_ON_RX_NOT_EMPTY | \
                                                     sdCard_SPI0_INT_ON_RX_OVER)
/* Nubmer of bits to receive/transmit */
#define sdCard_SPI0_BITCTR_INIT            (((uint8) (sdCard_SPI0_DATA_WIDTH << 1u)) - 1u)


/***************************************
*             Registers
***************************************/

#if(CY_PSOC3 || CY_PSOC5)
    #define sdCard_SPI0_TXDATA_REG (* (reg8 *) \
                                                sdCard_SPI0_BSPIM_sR8_Dp_u0__F0_REG)
    #define sdCard_SPI0_TXDATA_PTR (  (reg8 *) \
                                                sdCard_SPI0_BSPIM_sR8_Dp_u0__F0_REG)
    #define sdCard_SPI0_RXDATA_REG (* (reg8 *) \
                                                sdCard_SPI0_BSPIM_sR8_Dp_u0__F1_REG)
    #define sdCard_SPI0_RXDATA_PTR (  (reg8 *) \
                                                sdCard_SPI0_BSPIM_sR8_Dp_u0__F1_REG)
#else   /* PSOC4 */
    #if(sdCard_SPI0_USE_SECOND_DATAPATH)
        #define sdCard_SPI0_TXDATA_REG (* (reg16 *) \
                                          sdCard_SPI0_BSPIM_sR8_Dp_u0__16BIT_F0_REG)
        #define sdCard_SPI0_TXDATA_PTR (  (reg16 *) \
                                          sdCard_SPI0_BSPIM_sR8_Dp_u0__16BIT_F0_REG)
        #define sdCard_SPI0_RXDATA_REG (* (reg16 *) \
                                          sdCard_SPI0_BSPIM_sR8_Dp_u0__16BIT_F1_REG)
        #define sdCard_SPI0_RXDATA_PTR         (  (reg16 *) \
                                          sdCard_SPI0_BSPIM_sR8_Dp_u0__16BIT_F1_REG)
    #else
        #define sdCard_SPI0_TXDATA_REG (* (reg8 *) \
                                                sdCard_SPI0_BSPIM_sR8_Dp_u0__F0_REG)
        #define sdCard_SPI0_TXDATA_PTR (  (reg8 *) \
                                                sdCard_SPI0_BSPIM_sR8_Dp_u0__F0_REG)
        #define sdCard_SPI0_RXDATA_REG (* (reg8 *) \
                                                sdCard_SPI0_BSPIM_sR8_Dp_u0__F1_REG)
        #define sdCard_SPI0_RXDATA_PTR (  (reg8 *) \
                                                sdCard_SPI0_BSPIM_sR8_Dp_u0__F1_REG)
    #endif /* (sdCard_SPI0_USE_SECOND_DATAPATH) */
#endif     /* (CY_PSOC3 || CY_PSOC5) */

#define sdCard_SPI0_AUX_CONTROL_DP0_REG (* (reg8 *) \
                                        sdCard_SPI0_BSPIM_sR8_Dp_u0__DP_AUX_CTL_REG)
#define sdCard_SPI0_AUX_CONTROL_DP0_PTR (  (reg8 *) \
                                        sdCard_SPI0_BSPIM_sR8_Dp_u0__DP_AUX_CTL_REG)

#if(sdCard_SPI0_USE_SECOND_DATAPATH)
    #define sdCard_SPI0_AUX_CONTROL_DP1_REG  (* (reg8 *) \
                                        sdCard_SPI0_BSPIM_sR8_Dp_u1__DP_AUX_CTL_REG)
    #define sdCard_SPI0_AUX_CONTROL_DP1_PTR  (  (reg8 *) \
                                        sdCard_SPI0_BSPIM_sR8_Dp_u1__DP_AUX_CTL_REG)
#endif /* (sdCard_SPI0_USE_SECOND_DATAPATH) */

#define sdCard_SPI0_COUNTER_PERIOD_REG     (* (reg8 *) sdCard_SPI0_BSPIM_BitCounter__PERIOD_REG)
#define sdCard_SPI0_COUNTER_PERIOD_PTR     (  (reg8 *) sdCard_SPI0_BSPIM_BitCounter__PERIOD_REG)
#define sdCard_SPI0_COUNTER_CONTROL_REG    (* (reg8 *) sdCard_SPI0_BSPIM_BitCounter__CONTROL_AUX_CTL_REG)
#define sdCard_SPI0_COUNTER_CONTROL_PTR    (  (reg8 *) sdCard_SPI0_BSPIM_BitCounter__CONTROL_AUX_CTL_REG)

#define sdCard_SPI0_TX_STATUS_REG          (* (reg8 *) sdCard_SPI0_BSPIM_TxStsReg__STATUS_REG)
#define sdCard_SPI0_TX_STATUS_PTR          (  (reg8 *) sdCard_SPI0_BSPIM_TxStsReg__STATUS_REG)
#define sdCard_SPI0_RX_STATUS_REG          (* (reg8 *) sdCard_SPI0_BSPIM_RxStsReg__STATUS_REG)
#define sdCard_SPI0_RX_STATUS_PTR          (  (reg8 *) sdCard_SPI0_BSPIM_RxStsReg__STATUS_REG)

#define sdCard_SPI0_CONTROL_REG            (* (reg8 *) \
                                      sdCard_SPI0_BSPIM_BidirMode_SyncCtl_CtrlReg__CONTROL_REG)
#define sdCard_SPI0_CONTROL_PTR            (  (reg8 *) \
                                      sdCard_SPI0_BSPIM_BidirMode_SyncCtl_CtrlReg__CONTROL_REG)

#define sdCard_SPI0_TX_STATUS_MASK_REG     (* (reg8 *) sdCard_SPI0_BSPIM_TxStsReg__MASK_REG)
#define sdCard_SPI0_TX_STATUS_MASK_PTR     (  (reg8 *) sdCard_SPI0_BSPIM_TxStsReg__MASK_REG)
#define sdCard_SPI0_RX_STATUS_MASK_REG     (* (reg8 *) sdCard_SPI0_BSPIM_RxStsReg__MASK_REG)
#define sdCard_SPI0_RX_STATUS_MASK_PTR     (  (reg8 *) sdCard_SPI0_BSPIM_RxStsReg__MASK_REG)

#define sdCard_SPI0_TX_STATUS_ACTL_REG     (* (reg8 *) sdCard_SPI0_BSPIM_TxStsReg__STATUS_AUX_CTL_REG)
#define sdCard_SPI0_TX_STATUS_ACTL_PTR     (  (reg8 *) sdCard_SPI0_BSPIM_TxStsReg__STATUS_AUX_CTL_REG)
#define sdCard_SPI0_RX_STATUS_ACTL_REG     (* (reg8 *) sdCard_SPI0_BSPIM_RxStsReg__STATUS_AUX_CTL_REG)
#define sdCard_SPI0_RX_STATUS_ACTL_PTR     (  (reg8 *) sdCard_SPI0_BSPIM_RxStsReg__STATUS_AUX_CTL_REG)

#if(sdCard_SPI0_USE_SECOND_DATAPATH)
    #define sdCard_SPI0_AUX_CONTROLDP1     (sdCard_SPI0_AUX_CONTROL_DP1_REG)
#endif /* (sdCard_SPI0_USE_SECOND_DATAPATH) */


/***************************************
*       Register Constants
***************************************/

/* Status Register Definitions */
#define sdCard_SPI0_STS_SPI_DONE_SHIFT             (0x00u)
#define sdCard_SPI0_STS_TX_FIFO_EMPTY_SHIFT        (0x01u)
#define sdCard_SPI0_STS_TX_FIFO_NOT_FULL_SHIFT     (0x02u)
#define sdCard_SPI0_STS_BYTE_COMPLETE_SHIFT        (0x03u)
#define sdCard_SPI0_STS_SPI_IDLE_SHIFT             (0x04u)
#define sdCard_SPI0_STS_RX_FIFO_FULL_SHIFT         (0x04u)
#define sdCard_SPI0_STS_RX_FIFO_NOT_EMPTY_SHIFT    (0x05u)
#define sdCard_SPI0_STS_RX_FIFO_OVERRUN_SHIFT      (0x06u)

#define sdCard_SPI0_STS_SPI_DONE           ((uint8) (0x01u << sdCard_SPI0_STS_SPI_DONE_SHIFT))
#define sdCard_SPI0_STS_TX_FIFO_EMPTY      ((uint8) (0x01u << sdCard_SPI0_STS_TX_FIFO_EMPTY_SHIFT))
#define sdCard_SPI0_STS_TX_FIFO_NOT_FULL   ((uint8) (0x01u << sdCard_SPI0_STS_TX_FIFO_NOT_FULL_SHIFT))
#define sdCard_SPI0_STS_BYTE_COMPLETE      ((uint8) (0x01u << sdCard_SPI0_STS_BYTE_COMPLETE_SHIFT))
#define sdCard_SPI0_STS_SPI_IDLE           ((uint8) (0x01u << sdCard_SPI0_STS_SPI_IDLE_SHIFT))
#define sdCard_SPI0_STS_RX_FIFO_FULL       ((uint8) (0x01u << sdCard_SPI0_STS_RX_FIFO_FULL_SHIFT))
#define sdCard_SPI0_STS_RX_FIFO_NOT_EMPTY  ((uint8) (0x01u << sdCard_SPI0_STS_RX_FIFO_NOT_EMPTY_SHIFT))
#define sdCard_SPI0_STS_RX_FIFO_OVERRUN    ((uint8) (0x01u << sdCard_SPI0_STS_RX_FIFO_OVERRUN_SHIFT))

/* TX and RX masks for clear on read bits */
#define sdCard_SPI0_TX_STS_CLR_ON_RD_BYTES_MASK    (0x09u)
#define sdCard_SPI0_RX_STS_CLR_ON_RD_BYTES_MASK    (0x40u)

/* StatusI Register Interrupt Enable Control Bits */
/* As defined by the Register map for the AUX Control Register */
#define sdCard_SPI0_INT_ENABLE     (0x10u) /* Enable interrupt from statusi */
#define sdCard_SPI0_TX_FIFO_CLR    (0x01u) /* F0 - TX FIFO */
#define sdCard_SPI0_RX_FIFO_CLR    (0x02u) /* F1 - RX FIFO */
#define sdCard_SPI0_FIFO_CLR       (sdCard_SPI0_TX_FIFO_CLR | sdCard_SPI0_RX_FIFO_CLR)

/* Bit Counter (7-bit) Control Register Bit Definitions */
/* As defined by the Register map for the AUX Control Register */
#define sdCard_SPI0_CNTR_ENABLE    (0x20u) /* Enable CNT7 */

/* Bi-Directional mode control bit */
#define sdCard_SPI0_CTRL_TX_SIGNAL_EN  (0x01u)

/* Datapath Auxillary Control Register definitions */
#define sdCard_SPI0_AUX_CTRL_FIFO0_CLR         (0x01u)
#define sdCard_SPI0_AUX_CTRL_FIFO1_CLR         (0x02u)
#define sdCard_SPI0_AUX_CTRL_FIFO0_LVL         (0x04u)
#define sdCard_SPI0_AUX_CTRL_FIFO1_LVL         (0x08u)
#define sdCard_SPI0_STATUS_ACTL_INT_EN_MASK    (0x10u)

/* Component disabled */
#define sdCard_SPI0_DISABLED   (0u)


/***************************************
*       Macros
***************************************/

/* Returns true if componentn enabled */
#define sdCard_SPI0_IS_ENABLED (0u != (sdCard_SPI0_TX_STATUS_ACTL_REG & sdCard_SPI0_INT_ENABLE))

/* Retuns TX status register */
#define sdCard_SPI0_GET_STATUS_TX(swTxSts) ( (uint8)(sdCard_SPI0_TX_STATUS_REG | \
                                                          ((swTxSts) & sdCard_SPI0_TX_STS_CLR_ON_RD_BYTES_MASK)) )
/* Retuns RX status register */
#define sdCard_SPI0_GET_STATUS_RX(swRxSts) ( (uint8)(sdCard_SPI0_RX_STATUS_REG | \
                                                          ((swRxSts) & sdCard_SPI0_RX_STS_CLR_ON_RD_BYTES_MASK)) )


/***************************************
*       Obsolete definitions
***************************************/

/* Following definitions are for version compatibility.
*  They are obsolete in SPIM v2_30.
*  Please do not use it in new projects
*/

#define sdCard_SPI0_WriteByte   sdCard_SPI0_WriteTxData
#define sdCard_SPI0_ReadByte    sdCard_SPI0_ReadRxData
void  sdCard_SPI0_SetInterruptMode(uint8 intSrc)       ;
uint8 sdCard_SPI0_ReadStatus(void)                     ;
void  sdCard_SPI0_EnableInt(void)                      ;
void  sdCard_SPI0_DisableInt(void)                     ;

/* Obsolete register names. Not to be used in new designs */
#define sdCard_SPI0_TXDATA                 (sdCard_SPI0_TXDATA_REG)
#define sdCard_SPI0_RXDATA                 (sdCard_SPI0_RXDATA_REG)
#define sdCard_SPI0_AUX_CONTROLDP0         (sdCard_SPI0_AUX_CONTROL_DP0_REG)
#define sdCard_SPI0_TXBUFFERREAD           (sdCard_SPI0_txBufferRead)
#define sdCard_SPI0_TXBUFFERWRITE          (sdCard_SPI0_txBufferWrite)
#define sdCard_SPI0_RXBUFFERREAD           (sdCard_SPI0_rxBufferRead)
#define sdCard_SPI0_RXBUFFERWRITE          (sdCard_SPI0_rxBufferWrite)

#define sdCard_SPI0_COUNTER_PERIOD         (sdCard_SPI0_COUNTER_PERIOD_REG)
#define sdCard_SPI0_COUNTER_CONTROL        (sdCard_SPI0_COUNTER_CONTROL_REG)
#define sdCard_SPI0_STATUS                 (sdCard_SPI0_TX_STATUS_REG)
#define sdCard_SPI0_CONTROL                (sdCard_SPI0_CONTROL_REG)
#define sdCard_SPI0_STATUS_MASK            (sdCard_SPI0_TX_STATUS_MASK_REG)
#define sdCard_SPI0_STATUS_ACTL            (sdCard_SPI0_TX_STATUS_ACTL_REG)

#define sdCard_SPI0_INIT_INTERRUPTS_MASK  (sdCard_SPI0_INT_ON_SPI_DONE     | \
                                                sdCard_SPI0_INT_ON_TX_EMPTY     | \
                                                sdCard_SPI0_INT_ON_TX_NOT_FULL_DEF  | \
                                                sdCard_SPI0_INT_ON_RX_FULL      | \
                                                sdCard_SPI0_INT_ON_RX_NOT_EMPTY | \
                                                sdCard_SPI0_INT_ON_RX_OVER      | \
                                                sdCard_SPI0_INT_ON_BYTE_COMP)
                                                
/* Following definitions are for version Compatibility.
*  They are obsolete in SPIM v2_40.
*  Please do not use it in new projects
*/

#define sdCard_SPI0_DataWidth                  (sdCard_SPI0_DATA_WIDTH)
#define sdCard_SPI0_InternalClockUsed          (sdCard_SPI0_INTERNAL_CLOCK)
#define sdCard_SPI0_InternalTxInterruptEnabled (sdCard_SPI0_INTERNAL_TX_INT_ENABLED)
#define sdCard_SPI0_InternalRxInterruptEnabled (sdCard_SPI0_INTERNAL_RX_INT_ENABLED)
#define sdCard_SPI0_ModeUseZero                (sdCard_SPI0_MODE_USE_ZERO)
#define sdCard_SPI0_BidirectionalMode          (sdCard_SPI0_BIDIRECTIONAL_MODE)
#define sdCard_SPI0_Mode                       (sdCard_SPI0_MODE)
#define sdCard_SPI0_DATAWIDHT                  (sdCard_SPI0_DATA_WIDTH)
#define sdCard_SPI0_InternalInterruptEnabled   (0u)

#define sdCard_SPI0_TXBUFFERSIZE   (sdCard_SPI0_TX_BUFFER_SIZE)
#define sdCard_SPI0_RXBUFFERSIZE   (sdCard_SPI0_RX_BUFFER_SIZE)

#define sdCard_SPI0_TXBUFFER       sdCard_SPI0_txBuffer
#define sdCard_SPI0_RXBUFFER       sdCard_SPI0_rxBuffer

#endif /* (CY_SPIM_sdCard_SPI0_H) */


/* [] END OF FILE */
