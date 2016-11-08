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

#define f1000HZ 80000
#define MAX_SAMPLES 100
#define FS 1000
#define N 128

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
	uint32_t numSamples = 0;
	uint16_t adcValue = 0;
	uint16_t temperature = 0;
	
  PLL_Init(Bus80MHz);   				// 80 MHz
  UART_Init();              		// initialize UART device
	ADC0_InitTimer0ATriggerSeq3PD3(80000000/100);
	TxFifo_Init();
	EnableInterrupts();
	
	ST7735_InitR(INITR_REDTAB);
	ST7735_SetCursor(0,0); 
	ST7735_OutString("Temperature Data");
	ST7735_PlotClear(1000,4000);  // range from 0 to 4095
	ST7735_SetCursor(0,1); 
	ST7735_OutString("N=");
	ST7735_SetCursor(0,2); 
	ST7735_OutString("T= "); 
	ST7735_sDecOut2(2500);
  ST7735_OutString(" C");

	while(1){
		TxFifo_Get(&adcValue);
		temperature = 4000+(-3000*(long)adcValue)/4096;
		
		ST7735_PlotPoint(temperature);  // Measured temperature
    if((numSamples&(N-1))==0){          // fs sampling, fs/N samples plotted per second
      ST7735_PlotNextErase();  // overwrites N points on same line
    }
    if((numSamples%FS)==0){    // fs sampling, 1 Hz display of numerical data
      ST7735_SetCursor(3,1); 
			ST7735_OutUDec(adcValue);            // 0 to 4095
      ST7735_SetCursor(3,2); 
			ST7735_sDecOut2(temperature); // 0.01 C 
    }
    numSamples++;                       // counts the number of samples
	}
}


