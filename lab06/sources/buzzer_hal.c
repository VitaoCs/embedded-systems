/* ***************************************************************** */
/* File name:        buzzer_hal.c                                    */
/* File description: File dedicated to the hardware abstraction layer*/
/*                   related buzzer from the peripheral board        */
/* Author name:      dloubach                                        */
/* Creation date:    12jan2016                                       */
/* Revision date:    25fev2016                                       */
/* ***************************************************************** */

#include "buzzer_hal.h"
#include "es670_peripheral_board.h"

/* ************************************************ */
/* Method name:        buzzer_init                  */
/* Method description: Initialize the buzzer device */
/* Input params:       n/a                          */
/* Output params:      n/a                          */
/* ************************************************ */
void buzzer_init(void)
{
    /* un-gate port clock*/
    SIM_SCGC5 = SIM_SCGC5_PORTD(CGC_CLOCK_ENABLED);

    /* set pin as gpio */
    PORTD_PCR0 = PORT_PCR_MUX(BUZZER_ALT);

    /* set pin as digital output */
    GPIOD_PDDR |= GPIO_PDDR_PDD(BUZZER_PIN);

}



/* ************************************************ */
/* Method name:        buzzer_clearBuzz             */
/* Method description: Clear the buzzer             */
/* Input params:       n/a                          */
/* Output params:      n/a                          */
/* ************************************************ */
void buzzer_clearBuzz(void)
{
    /* clear desired led */
    GPIOD_PCOR = GPIO_PCOR_PTCO(0x01);

}



/* ************************************************ */
/* Method name:        buzzer_setBuz                */
/* Method description: Set the buzze                */
/* Input params:       n/a                          */
/* Output params:       n/a                         */
/* ************************************************ */
void buzzer_setBuzz(void)
{
    /* set desired led */
    GPIOD_PSOR = GPIO_PSOR_PTSO(0x01);

}
