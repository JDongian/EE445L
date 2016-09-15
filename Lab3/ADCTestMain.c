// ADCTestMain.c
// Runs on TM4C123
// This program periodically samples ADC channel 0 and stores the
// result to a global variable that can be accessed with the JTAG
// debugger and viewed with the variable watch feature.
// Daniel Valvano
// September 5, 2015

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
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

// center of X-ohm potentiometer connected to PE3/AIN0
// bottom of X-ohm potentiometer connected to ground
// top of X-ohm potentiometer connected to +3.3V 
#include <stdint.h>
#include "ADCSWTrigger.h"
#include "../inc/tm4c123gh6pm.h"
#include "PLL.h"
#include "ST7735.h"
#include "fixed.h"
#include "Timer1.h"
#include "../PeriodicTimer2AInts_4C123/Timer2.h"
#include "histogram.h"
#include <stdio.h>      /* printf, scanf, NULL */
#include <stdlib.h>     /* calloc, exit, free */
#include <string.h>     /* memset */

#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
#define LEDS      (*((volatile uint32_t *)0x40025038))
	
#define MAX_RECORDS 1000
#define NUM_ADC_OUTPUTS 4095
#define ADC_HW_AVG_NONE 0
#define ADC_HW_AVG_4X 0x2
#define ADC_HW_AVG_16X 0x4
#define ADC_HW_AVG_64X 0x6

#define PLOT_MARGIN 10

#define PF4   (*((volatile uint32_t *)0x40025040))

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

volatile uint32_t ADCvalue;
volatile uint32_t ADCRecords[MAX_RECORDS];
volatile uint32_t timeRecords[MAX_RECORDS];
volatile uint16_t recordIndex;

uint16_t color = ST7735_MAGENTA;

// This debug function initializes Timer0A to request interrupts
// at a 100 Hz frequency.  It is similar to FreqMeasure.c.
void Timer0A_Init100HzInt(void){
  volatile uint32_t delay;
  DisableInterrupts();
  // **** general initialization ****
  SYSCTL_RCGCTIMER_R |= 0x01;      // activate timer0
  delay = SYSCTL_RCGCTIMER_R;      // allow time to finish activating
  TIMER0_CTL_R &= ~TIMER_CTL_TAEN; // disable timer0A during setup
  TIMER0_CFG_R = 0;                // configure for 32-bit timer mode
  // **** timer0A initialization ****
                                   // configure for periodic mode
  TIMER0_TAMR_R = TIMER_TAMR_TAMR_PERIOD;
  TIMER0_TAILR_R = 799999*16;         // start value for 100 Hz interrupts
  TIMER0_IMR_R |= TIMER_IMR_TATOIM;// enable timeout (rollover) interrupt
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;// clear timer0A timeout flag
  TIMER0_CTL_R |= TIMER_CTL_TAEN;  // enable timer0A 32-b, periodic, interrupts
  // **** interrupt initialization ****
                                   // Timer0A=priority 2
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x40000000; // top 3 bits
  NVIC_EN0_R = 1<<19;              // enable interrupt 19 in NVIC
}
void Timer0A_Handler(void){
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;    // acknowledge timer0A timeout
  PF2 ^= 0x04;                   // profile
  PF2 ^= 0x04;                   // profile
  ADCvalue = ADC0_InSeq3();
  PF2 ^= 0x04;                   // profile
	
	// stop recording after 1000 records
	if(recordIndex < 1000){
		ADCRecords[recordIndex] = ADCvalue; // record ADC value into array
		timeRecords[recordIndex] = TIMER1_TAR_R; // record time into array
		++recordIndex;
	}
	ST7735_FillScreen(color);
					if (color == ST7735_MAGENTA) color = ST7735_WHITE;
				  else color = ST7735_MAGENTA;
}


void UserTask(void){
}

void UserTask2(void){}


void init_hw(void){
	volatile uint32_t delay;
	
	PLL_Init(Bus80MHz);              // bus clock at 80 MHz
	SYSCTL_RCGCGPIO_R |= 0x20;       // activate port F
  delay = SYSCTL_RCGCGPIO_R;       // allow time to finish activating?
  ADC0_InitSWTriggerSeq3_Ch9();    // allow time to finish activating
  Timer0A_Init100HzInt();          // set up Timer0A for 100 Hz interrupts
  GPIO_PORTF_DIR_R |= 0x6;        // make PF2, PF1 out (built-in LED)
  GPIO_PORTF_AFSEL_R &= ~0x16;     // disable alt funct on PF2, PF1, PF4
  GPIO_PORTF_DEN_R |= 0x16;        // enable digital I/O on PF2, PF1, PF4
                                   // configure PF2 as GPIO
  GPIO_PORTF_PUR_R |= 0x10;         // pullup for PF4
  GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFF000F)+0x00000000;
  GPIO_PORTF_AMSEL_R = 0;          // disable analog functionality on PF
  LEDS = 0;                        // turn LEDs off
	
  ST7735_InitR(INITR_REDTAB);				//LCD init
	
  Timer1_Init(&UserTask, 0xFFFFFFFF); // initialize timer1 (slowest rate)
	Timer2_Init(&UserTask2, 7920); // initialize timer2 to ~99 us period

  EnableInterrupts();
}

void DelayWait10ms(uint32_t n){
	uint32_t volatile time;
  while(n){
    time = 727240*2/91;  // 10msec
    while(time){
	  	time--;
    }
    n--;
  }
}

void Pause(void){
  while(PF4==0x00){ 
    DelayWait10ms(10);
  }
  while(PF4==0x10){
    DelayWait10ms(10);
  }
}

void displayRunMode(uint8_t runMode) {
    ADC0_SAC_R = runMode; // configures ADC hardware averaging 
    switch (runMode) {
        case ADC_HW_AVG_NONE: 
            ST7735_OutString("ADC Averaging: none");
            break;
        case ADC_HW_AVG_4X:
            ST7735_OutString("ADC Averaging: 4x");
            break;
        case ADC_HW_AVG_16X:
            ST7735_OutString("ADC Averaging: 16x");
            break;
        case ADC_HW_AVG_64X:
            ST7735_OutString("ADC Averaging: 64x");
            break;
        default: 
            break;
    }
}


/*
 * Convenience function to cycle through 
 * different ADC averaging configurations
 */
uint8_t nextRunMode(uint8_t mode) {
    return (mode + 2) % 8;
}


int main(void){
  init_hw();

  uint8_t runMode = ADC_HW_AVG_NONE; // initial hardware averaging configuration 
  volatile uint32_t jitter;
 
  while(1) {
		
      recordIndex = 0;
		
      // Reset numOucrrences counts to 0 for next run
      resetNumOcurrences();
      EnableInterrupts();
			
			// reset the screen
      ST7735_FillScreen(0);
      ST7735_SetCursor(0,0);
		
      ST7735_OutString("ADC PMF\n");
      displayRunMode(runMode); // Print what HW avg mode is being used to screen
			
			ST7735_drawLine(point_new(40,40), point_new(40,70), ST7735_RED);
			ST7735_drawLine(point_new(70,40), point_new(70,70), ST7735_RED);
			ST7735_drawLine(point_new(20,80), point_new(40,90), ST7735_RED);
			ST7735_drawLine(point_new(40,90), point_new(70,90), ST7735_RED);
			ST7735_drawLine(point_new(70,90), point_new(90,80), ST7735_RED);
			
		
			point star[10];
			star[0] = point_new(50 + 0, 50 + 4);
			star[1] = point_new(50 + 4, 50 + 4);
			star[2] = point_new(50 + 6, 50 + 0);
			star[3] = point_new(50 + 8, 50 + 4);
			star[4] = point_new(50 + 12, 50 + 4);
			star[5] = point_new(50 + 9, 50 + 7);
			star[6] = point_new(50 + 11, 50 + 8);
			star[7] = point_new(50 + 6, 50 + 6);
			star[8] = point_new(50 + 2, 50 + 8);
			star[9] = point_new(50 + 3, 50 + 7);
			drawPolygon(star, 10, ST7735_WHITE);
		
      while(recordIndex < MAX_RECORDS) {
          // toggle LED

      }
      DisableInterrupts();

      jitter = calculateJitter(timeRecords, MAX_RECORDS);

      ST_drawData(ADCRecords, MAX_RECORDS);
			
			runMode = nextRunMode(runMode);	
			
      Pause();
  }
}


