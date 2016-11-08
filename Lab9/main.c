// main.c
// Runs on LM4F120/TM4C123
// UART runs at 115,200 baud rate 
// Daniel Valvano
// May 3, 2015

/* This example accompanies the books
  "Embedded Systems: Introduction to ARM Cortex M Microcontrollers",
  ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2015

"Embedded Systems: Real Time Interfacing to ARM Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2015
 
 Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */


#include <stdint.h> // C99 variable types
#include <stdio.h>
#include "../inc/tm4c123gh6pm.h"
#include "ADCSWTrigger.h"
#include "uart.h"
#include "PLL.h"
#include "Timers.h"
#include "ADCT0ATrigger.h"
#include "FIFO.h"
#include "ST7735.h"
#include "fixed.h"
#include "Temperature.h"

#define f100HZ 80000000/100
#define MAX_SAMPLES 100

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode


volatile uint32_t ADCRecords[MAX_SAMPLES];
uint16_t adcValue = 0;
volatile uint32_t numSamples = 0;

int main(void){ 
  PLL_Init(Bus80MHz); 	// 80 MHz
  UART_Init();      		// initialize UART device
	//initialize ADC, hardware timer trigger, 100Hz sampling
	ADC0_InitTimer0ATriggerSeq3PD3(f100HZ);  
	TxFifo_Init();	// initialize FIFO
	ST7735_InitR(INITR_REDTAB); // initialize LCD
	ST7735_InitTemperatureGraph();	// initialize graph area
	
	EnableInterrupts();

	while(1){
		TxFifo_Get(&adcValue);	// get most recent ADC value from FIFO
		ST7735_UpdateTemperatureGraph(numSamples, adcValue); // plot new point and display ADC and temperature values
    numSamples++;                       // counts the number of samples
	}
}


