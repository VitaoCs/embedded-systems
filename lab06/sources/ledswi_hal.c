/* ***************************************************************** */
/* File name:        ledswi_hal.c                                    */
/* File description: This file has a couple of useful functions to   */
/*                   control LEDs and Switches from peripheral board */
/* Author name:      dloubach                                        */
/* Creation date:    20jan2015                                       */
/* Revision date:    25fev2016                                       */
/* ***************************************************************** */

#include "ledswi_hal.h"
#include "es670_peripheral_board.h"

#define USING_OPENSDA_DEBUG

/* ************************************************ */
/* Method name:        ledswi_initLedSwitch         */
/* Method description: As the hardware board was    */
/*                     designed with LEDs/Switches  */
/*                     sharing the same pins, this  */
/*                     method configures how many   */
/*                     LEDS and switches will be    */
/*                     available for the application*/
/* Input params:       cLedNum    = num of LEDs     */
/*                     cSwitchNum = num of Switches */
/*                     cLedNum +                    */
/*                     cSwitchNum <= MAX_LED_SWI    */
/* Outpu params:       n/a                          */
/* ************************************************ */
void ledswi_initLedSwitch(char cLedNum, char cSwitchNum)
{
    /* un-gate port clock*/
    SIM_SCGC5 = SIM_SCGC5_PORTA(CGC_CLOCK_ENABLED);

    /* set pin as gpio */
#ifndef USING_OPENSDA_DEBUG
    PORTA_PCR1 = PORT_PCR_MUX(LS1_ALT);
    PORTA_PCR2 = PORT_PCR_MUX(LS2_ALT);
#endif
    PORTA_PCR4 = PORT_PCR_MUX(LS3_ALT);
    PORTA_PCR5 = PORT_PCR_MUX(LS4_ALT);


    /* check if the number to configured is according to
    hardware dev kit */
    if((cLedNum + cSwitchNum) <= MAX_LED_SWI)
    {
        /* max number of peripherals to configure is ok, carry on */
        switch(cSwitchNum)
        {
            case 0:
                /* no switches in system configuration */
                /* all leds */
                GPIOA_PDDR |= GPIO_PDDR_PDD(LS1_DIR_OUTPUT | LS2_DIR_OUTPUT | LS3_DIR_OUTPUT | LS4_DIR_OUTPUT);
                break;

            case 1:
                /* just 1 switch */
                GPIOA_PDDR |= GPIO_PDDR_PDD(LS2_DIR_OUTPUT | LS3_DIR_OUTPUT | LS4_DIR_OUTPUT);
                GPIOA_PDDR &= ~GPIO_PDDR_PDD(LS1_DIR_INPUT);
                break;

            case 2:
                /* just 2 switches */
                GPIOA_PDDR |= GPIO_PDDR_PDD(LS3_DIR_OUTPUT | LS4_DIR_OUTPUT);
                GPIOA_PDDR &= ~GPIO_PDDR_PDD(LS1_DIR_INPUT | LS2_DIR_INPUT);
                break;

            case 3:
                /* 3 switches */
                GPIOA_PDDR |= GPIO_PDDR_PDD(LS4_DIR_OUTPUT);
                GPIOA_PDDR &= ~GPIO_PDDR_PDD(LS1_DIR_INPUT | LS2_DIR_INPUT | LS3_DIR_INPUT);
                break;

            case 4:
                /* 4 switches */
                GPIOA_PDDR &= ~GPIO_PDDR_PDD(LS1_DIR_INPUT | LS2_DIR_INPUT | LS3_DIR_INPUT | LS4_DIR_INPUT);
                break;
        } /* switch(cSwitchNum) */

    } /* if((cLedNum + cSwitchNum) <= MAX_LED_SWI) */

}



/* ************************************************ */
/* Method name:        ledswi_setLed                */
/* Method description: set the led ON               */
/* Input params:       cLedNum    = which LED {1..4}*/
/* Outpu params:       n/a                          */
/* ************************************************ */
void ledswi_setLed(char cLedNum)
{
    /* sanity check */
    if(cLedNum <= MAX_LED_SWI)
    {
        switch(cLedNum)
        {
            case 1:
                GPIOA_PSOR = GPIO_PSOR_PTSO( (0x01U << LS1_PIN) );
                break;
            case 2:
                GPIOA_PSOR = GPIO_PSOR_PTSO( (0x01U << LS2_PIN) );
                break;
            case 3:
                GPIOA_PSOR = GPIO_PSOR_PTSO( (0x01U << LS3_PIN) );
                break;
            case 4:
                GPIOA_PSOR = GPIO_PSOR_PTSO( (0x01U << LS4_PIN) );
                break;
        } /* switch(cLedNum) */

    } /* if(cLedNum <= MAX_LED_SWI) */
}



/* ************************************************ */
/* Method name:        ledswi_clearLed              */
/* Method description: set the led OFF              */
/* Input params:       cLedNum    = which LED {1..4}*/
/* Outpu params:       n/a                          */
/* ************************************************ */
void ledswi_clearLed(char cLedNum)
{
    /* sanity check */
    if(cLedNum <= MAX_LED_SWI)
    {
        switch(cLedNum)
        {
            case 1:
                GPIOA_PCOR = GPIO_PCOR_PTCO( (0x01U << LS1_PIN) );
                break;
            case 2:
                GPIOA_PCOR = GPIO_PCOR_PTCO( (0x01U << LS2_PIN) );
                break;
            case 3:
                GPIOA_PCOR = GPIO_PCOR_PTCO( (0x01U << LS3_PIN) );
                break;
            case 4:
                GPIOA_PCOR = GPIO_PCOR_PTCO( (0x01U << LS4_PIN) );
                break;
        } /* switch(cLedNum) */

    } /* if(cLedNum <= MAX_LED_SWI) */
}



/* ************************************************ */
/* Method name:        ledswi_getSwitchStatus       */
/* Method description: return the switch status     */
/* Input params:       cSwitchNum = which switch    */
/* Outpu params:       switch_status_type_e         */
/* ************************************************ */
switch_status_type_e ledswi_getSwitchStatus(char cSwitchNum)
{
    switch_status_type_e sstReturn = SWITCH_OFF;

    /* sanity check */
    if(cSwitchNum <= MAX_LED_SWI)
    {
        switch(cSwitchNum)
        {
            case 1:
                if(SWITCH_ON == ((GPIOA_PDIR & LS1_DIR_INPUT) >> LS1_PIN) )
                    sstReturn = SWITCH_ON;
                break;

            case 2:
                if(SWITCH_ON == ((GPIOA_PDIR & LS2_DIR_INPUT) >> LS2_PIN) )
                    sstReturn = SWITCH_ON;
                break;

            case 3:
                if(SWITCH_ON == ((GPIOA_PDIR & LS3_DIR_INPUT) >> LS3_PIN) )
                    sstReturn = SWITCH_ON;
                break;

            case 4:
                if(SWITCH_ON == ((GPIOA_PDIR & LS4_DIR_INPUT) >> LS4_PIN) )
                    sstReturn = SWITCH_ON;
                break;

        } /* switch(cSwitchNum) */

    } /* if(cSwitchNum <= MAX_LED_SWI) */

    /* return the result */
    return(sstReturn);

}
