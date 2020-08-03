/* ***************************************************************** */
/* File name:        debugUart.c                                     */
/* File description: Debugging through UART interface                */
/* Author name:      dloubach                                        */
/* Creation date:    22out2015                                       */
/* Revision date:    25fev2016                                       */
/* ***************************************************************** */

#include "debugUart.h"

/* system includes */
#include "fsl_clock_manager.h"
#include "fsl_device_registers.h"
#include "fsl_port_hal.h"
#include "fsl_smc_hal.h"
#include "fsl_debug_console.h"


/* The UART to use for debug messages */
#ifndef BOARD_DEBUG_UART_INSTANCE
    #define BOARD_DEBUG_UART_INSTANCE   0
    #define BOARD_DEBUG_UART_BASEADDR   UART0
#endif
#ifndef BOARD_DEBUG_UART_BAUD
    #define BOARD_DEBUG_UART_BAUD       115200
#endif



/* ************************************************ */
/* Method name:        debugUart_init               */
/* Method description: Initialize the UART0 as debug*/
/* Input params:       n/a                          */
/* Output params:      n/a                          */
/* ************************************************ */
void debugUart_init(void)
{
    /* LPSCI0 */
    /* UART0_RX */
    PORT_HAL_SetMuxMode(PORTA, 1u, kPortMuxAlt2);
    /* UART0_TX */
    PORT_HAL_SetMuxMode(PORTA, 2u, kPortMuxAlt2);


    /* Select different clock source for LPSCI */
    CLOCK_SYS_SetLpsciSrc(BOARD_DEBUG_UART_INSTANCE, kClockLpsciSrcPllFllSel);

    /* init the debug console */
    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUD, kDebugConsoleLPSCI);
}
