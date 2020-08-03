/* **********************************************/
/* File name: main.c 							*/
/* File description: This file implements the 	*/
/* functions needed to perform the requisits of	*/
/* the lab02 from ES670							*/
/* Author name: Laura Marchione RA:156169		*/
/* and 			Victor Cintra Santos RA:157461	*/
/* Creation date: 12mar2019 					*/
/* Revision date: 19mar2019 					*/
/* **********************************************/

#include "buzzer_hal.h"
#include "es670_peripheral_board.h"
#include "ledswi_hal.h"
#include "mcg_hal.h"
#include "util.h"
#include "debugUart.h"
#include "fsl_debug_console.h"
#include "print_scan.h"
#include <MKL25Z4.h>

/* Method Name: defaultErrorMessage		*/
/* Method description: print the default*/
/* error message on the target			*/
/* Input params: void					*/
/* Output params: void					*/
void defaultErrorMessage(){
	PUTCHAR('E');
	PUTCHAR('R');
	PUTCHAR('R');
}

/* Method Name: defaultACKMessage		*/
/* Method description: print the default*/
/* acknowledge message on the target	*/
/* Input params: void					*/
/* Output params: void					*/
void defaultACKMessage(){
	PUTCHAR('A');
	PUTCHAR('C');
	PUTCHAR('K');
}

/* Method Name: ungateDisplay7Seg		*/
/* Method description: enable the port,	*/
/* clock and recorders of our target	*/
/* Input params: void					*/
/* Output params: void					*/
void ungateDisplay7Seg(){
	SIM_SCGC5 = SIM_SCGC5_PORTC(0b11110011111111); // Liberacao do clock (ungate) da porta C e seus respectivos registradores
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

/* Method Name: display7SegController	*/
/* Method description: receives the		*/
/* number display and the number that	*/
/* will be written on it; returns the 	*/
/* binary to control the display		*/
/* Input params: displayNumChar, numChar*/
/* Output params: displaySignal			*/
int display7SegController (char displayNumChar, char numChar){
	int displayNum = displayNumChar - '0'; //Passamos os valores de char para int e para os indices dos vetores
	int num = numChar - '0';
    int displaySignal;						//Retorno da funcao

    char displayOptions [5];    			// Vetor com os valores em binario dos enables dos displays
	displayOptions [1] = 0b00100000;
	displayOptions [2] = 0b00010000;
	displayOptions [3] = 0b00001000;
	displayOptions [4] = 0b00000100;
    char displayNumbers [10];   			// Vetor com os valores em binario dos numeros em segmentos
	displayNumbers [0] = 0b00111111;
	displayNumbers [1] = 0b00000110;
	displayNumbers [2] = 0b01011011;
	displayNumbers [3] = 0b01001111;
	displayNumbers [4] = 0b01100110;
	displayNumbers [5] = 0b01101101;
	displayNumbers [6] = 0b01111101;
	displayNumbers [7] = 0b00100111;
	displayNumbers [8] = 0b01111111;
	displayNumbers [9] = 0b01101111;

	int disp = displayOptions[displayNum];
	disp =  disp  << 8;
	int numInt = displayNumbers[num];
	displaySignal = disp | numInt;			// Criamos o binário com o enable e número a ser escrito

    return displaySignal;
}

/* Method Name: dataTargetCommand		*/
/* Method description: receives the		*/
/* letter wich is read on the program	*/
/* that communicates with the target 	*/
/* and controls our state machine		*/
/* Input params: letter					*/
/* Output params: void					*/
void dataTargetCommand(char letter){
	static int counter = 0;
	static char switchStatus;
	static char display;
	int signalDisplay;

	if (counter == 0){
		if (letter == 'L'){
			counter = 1;
		}
		else if (letter == 'S'){
			counter = 6;
		}
		else if (letter == 'D'){
			counter = 8;
		}
		else{
			counter = 0;
			defaultErrorMessage();
		}
	}
	else if (counter == 1){
		if (letter == 'S'){
			counter = 2;
		}
		else if (letter == 'C'){
			counter = 3;
		}
		else{
			counter = 0;
			defaultErrorMessage();
		}
	}
	else if (counter == 2){
		if (letter == '4'){
			counter = 0;
			ledswi_setLed(4u);
			defaultACKMessage();
		}
		else{
			counter = 0;
			defaultErrorMessage();
		}
	}
	else if (counter == 3){
		if (letter == '4'){
			counter = 0;
			ledswi_clearLed(4u);
			defaultACKMessage();
		}
		else{
			counter = 0;
			defaultErrorMessage();
		}
	}
	else if (counter == 6){
		if (letter == '3'){
			counter = 0;
			switchStatus = ledswi_getSwitchStatus(3u);
			defaultACKMessage();
		}
		else{
			counter = 0;
			defaultErrorMessage();
		}
	}

	else if (counter == 8){
		if (letter == '1'||'2'||'3'||'4'){
			counter = 9;
			display = letter;
		}
		else{
			counter = 0;
			defaultErrorMessage();
		}
	}
	else if (counter == 9){
		if(letter == 'C'){
			counter = 0;
			GPIOC_PCOR = GPIO_PCOR_PTCO(0b11111111111111);
			defaultACKMessage();
		}
		else if (letter == '0'||'1'||'2'||'3'||'4'||'5'||'6'||'7'||'8'||'9'){
			counter = 0;
			signalDisplay = display7SegController (display, letter);
			util_genDelay088us();
			GPIOC_PCOR = GPIO_PCOR_PTCO(0b11111111111111);
			GPIOC_PSOR = GPIO_PSOR_PTSO(signalDisplay);
			defaultACKMessage();
		}

		else {
			counter = 0;
			defaultErrorMessage();
		}
	}

}

int main(void){

	int dataTarget;

	mcg_clockInit();	// Inicializamos o clock da placa
	debugUart_init();	// Inicializamos o debug UART
	ledswi_initLedSwitch(1u, 3u);
	ungateDisplay7Seg();

	for(;;){
		dataTarget = GETCHAR();
		dataTargetCommand(dataTarget);
	}

	return(0);
}