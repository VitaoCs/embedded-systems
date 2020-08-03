/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "buzzer_hal.h"
#include "es670_peripheral_board.h"
#include "ledswi_hal.h"
#include "mcg_hal.h"
#include "util.h"
#include <MKL25Z4.h>

void ungateDisplay7Seg(){
	SIM_SCGC5 = SIM_SCGC5_PORTC(0b11110011111111); // Liberação do clock (ungate) da porta C e seus respectivos registradores
	PORTC_PCR0 = PORT_PCR_MUX(0x01); // Configurando registradores do display de 7seg da porta C como GPIO
	PORTC_PCR1 = PORT_PCR_MUX(0x01); //.
	PORTC_PCR2 = PORT_PCR_MUX(0x01); //.
	PORTC_PCR3 = PORT_PCR_MUX(0x01); //.
	PORTC_PCR4 = PORT_PCR_MUX(0x01); //.
	PORTC_PCR5 = PORT_PCR_MUX(0x01); //.
	PORTC_PCR6 = PORT_PCR_MUX(0x01); //.
	PORTC_PCR7 = PORT_PCR_MUX(0x01); //.
	PORTC_PCR13 = PORT_PCR_MUX(0x01); // Configurando enable display 01
	PORTC_PCR12 = PORT_PCR_MUX(0x01); // Configurando enable display 02
	PORTC_PCR11 = PORT_PCR_MUX(0x01); // Configurando enable display 03
	PORTC_PCR10 = PORT_PCR_MUX(0x01); // Configurando enable display 04
	GPIOC_PDDR = GPIO_PDDR_PDD(0b11110011111111); // Configurando registradores da porta C como outputs
}

int main(void){

	/* Tarefa 01 */
	char switchStatus;
	mcg_clockInit();
	ledswi_initLedSwitch(1u, 3u);
	ledswi_setLed(4u);
	switchStatus = ledswi_getSwitchStatus(3u);

	/* Tarefa 02 */
	ungateDisplay7Seg();

	for(;;){
		GPIOC_PSOR = GPIO_PSOR_PTSO(0b10000001100110);
		util_genDelay088us();
		GPIOC_PCOR = GPIO_PCOR_PTCO(0b10000001100110);

		GPIOC_PSOR = GPIO_PSOR_PTSO(0b01000001011011);
		util_genDelay088us();
		GPIOC_PCOR = GPIO_PCOR_PTCO(0b01000001011011);

		GPIOC_PSOR = GPIO_PSOR_PTSO(0b00100001100110);
		util_genDelay088us();
		GPIOC_PCOR = GPIO_PCOR_PTCO(0b00100001100110);

		GPIOC_PSOR = GPIO_PSOR_PTSO(0b00010001011011);
		util_genDelay088us();
		GPIOC_PCOR = GPIO_PCOR_PTCO(0b00010001011011);
	}
	return 0;
}
