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

#define f1000HZ 80000
#define MAX_SAMPLES 100

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

volatile uint32_t ADCdata;
volatile uint32_t ADCRecords[MAX_SAMPLES];
volatile uint16_t sampleIndex = 0;

void Timer0A_Handler(void){
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;    // acknowledge timer0A timeout
  ADCdata = ADC0_InSeq3();
	
	// stop recording after 100 samples
	if(sampleIndex < 100){
		ADCRecords[sampleIndex] = ADCdata; // record ADC value into array
		++sampleIndex;
	}
}

int main(void){ 
	char outBuf[200];
  PLL_Init(Bus80MHz);   				// 80 MHz
  UART_Init();              		// initialize UART device
  ADC0_InitSWTriggerSeq3_Ch9();	// initialize ADC0
	Timer0A_Init(f1000HZ);				// initialize Timer0A, 1000Hz interrupts
	EnableInterrupts();
  while(sampleIndex < MAX_SAMPLES) {} // wait for ADC sample buffer to fill
  DisableInterrupts();
	// print ADC data
	for (int i = 0; i < MAX_SAMPLES; i++){
		sprintf(outBuf, "Sample 0: %d\n\r", ADCRecords[i]);
		UART_OutString(outBuf);
	}
}


