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
#include <stdio.h>      /* printf, scanf, NULL */
#include <stdlib.h>     /* calloc, exit, free */
#include <string.h>     /* memset */

#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
#define LEDS      (*((volatile uint32_t *)0x40025038))
#define RED       0x02
#define BLUE      0x04
#define GREEN     0x08
#define WHEELSIZE 8           // must be an integer multiple of 2
                              //    red, yellow,    green, light blue, blue, purple,   white,          dark
#define MAX_RECORDS 1000
#define NUM_ADC_OUTPUTS 4095
#define ADC_HW_AVG_NONE 0
#define ADC_HW_AVG_4X 0x2
#define ADC_HW_AVG_16X 0x4
#define ADC_HW_AVG_64X 0x6

#define PF4   (*((volatile uint32_t *)0x40025040))
const uint32_t COLORWHEEL[WHEELSIZE] = {RED, RED+GREEN, GREEN, GREEN+BLUE, BLUE, BLUE+RED, RED+GREEN+BLUE, 0};

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

volatile uint32_t ADCvalue;
volatile uint32_t ADC_records[MAX_RECORDS];
volatile uint32_t time_records[MAX_RECORDS];
volatile int32_t numOcurrences[NUM_ADC_OUTPUTS];
volatile uint16_t record_index;
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
  TIMER0_TAILR_R = 799999;         // start value for 100 Hz interrupts
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
	if(record_index < 1000){
		ADC_records[record_index] = ADCvalue; // record ADC value into array
		time_records[record_index] = TIMER1_TAR_R; // record time into array
		++record_index;
	}
}


void UserTask(void){
  static uint32_t i = 0;
  LEDS = COLORWHEEL[i&(WHEELSIZE-1)];
  i = i + 1;
}


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
	ST7735_FillScreen(0);  // set screen to black
  ST7735_SetCursor(0,0);
	
  Timer1_Init(&UserTask, 0); // initialize timer1 (16 Hz)
//  Timer1_Init(&UserTask, 5000000); // initialize timer1 (16 Hz)
//  Timer1_Init(&UserTask, 4000);    // initialize timer2 (20,000 Hz)
//  Timer1_Init(&UserTask, 80000000);// initialize timer1 (1 Hz)
//  Timer1_Init(&UserTask, 0xFFFFFFFF); // initialize timer1 (slowest rate)

  EnableInterrupts();
}


double calculate_jitter(volatile uint32_t times[], uint16_t length) {
	uint32_t max = 0;
	uint32_t min = (uint32_t)-1; // max uint32_t value
	
	// assert more than 1 entry
	for (int i = 0; i < length-1; ++i) {
		uint32_t diff = times[i+1] - times[i];
		if (diff < min) {
			min = diff;
		}
		if (diff > max) {
			max = diff;
		}
	}
	
	return max - min;
}


typedef struct histogram {
	uint32_t min;
	uint32_t max;
	uint32_t* freq;
} histogram;


void histogramify(volatile uint32_t data[], uint16_t length, histogram* h) {
	uint32_t min = data[0];
	uint32_t max = 0;
	uint16_t index;
	
	for (int i = 0; i < length; ++i) {
		if (data[i] < min) {
			min = data[i];
		}
		if (max < data[i]) {
			max = data[i];
		}
		index = data[i];
		numOcurrences[index]++;
	}
	//assert(min <= max)
	h->min = min;
	h->max = max;

	//h->freq = malloc((max - min + 1) * sizeof(uint32_t));
	/*
	for (int i = 0; i < length; ++i) {
		uint32_t offset = data[i] - min;
		++(h->freq[offset]);
	}
	*/
}


void draw_data(volatile uint32_t data[], uint16_t length) {
   // ST7735_XYplot(50,(int32_t *)StarXbuf,(int32_t *)StarYbuf);
   // Pause();
	histogram adc_hist;
	histogramify(data, length, &adc_hist);
	
	ST7735_XYplotInit("", adc_hist.min, adc_hist.max, 0, 500);
	
	for (int i = adc_hist.min; i <= adc_hist.max; ++i) {
		ST7735_PlotBar_Lab2(i, numOcurrences[i]);
	}
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
	

int main(void){
  init_hw();
	/* original code
	PLL_Init(Bus80MHz);                   // 80 MHz
  SYSCTL_RCGCGPIO_R |= 0x20;            // activate port F
  ADC0_InitSWTriggerSeq3_Ch9();         // allow time to finish activating
  Timer0A_Init100HzInt();               // set up Timer0A for 100 Hz interrupts
  GPIO_PORTF_DIR_R |= 0x06;             // make PF2, PF1 out (built-in LED)
  GPIO_PORTF_AFSEL_R &= ~0x06;          // disable alt funct on PF2, PF1
  GPIO_PORTF_DEN_R |= 0x06;             // enable digital I/O on PF2, PF1
                                        // configure PF2 as GPIO
  GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFFF00F)+0x00000000;
  GPIO_PORTF_AMSEL_R = 0;               // disable analog functionality on PF
  PF2 = 0;                      // turn off LED
  EnableInterrupts();
	*/
	uint8_t runNum = 0;
  while(1){
		ST7735_FillScreen(0);  // set screen to black
		ST7735_SetCursor(0,0);
		ST7735_OutString("ADC PMF\n");
		switch (runNum) {
			case 0: 
				ADC0_SAC_R = ADC_HW_AVG_NONE;
				ST7735_OutString("ADC Averaging: none");
				break;
			case 1:
				ADC0_SAC_R = ADC_HW_AVG_4X;
				ST7735_OutString("ADC Averaging: 4x");
				break;
			case 2:
				ADC0_SAC_R = ADC_HW_AVG_16X;
				ST7735_OutString("ADC Averaging: 16x");
				break;
			case 3:
				ADC0_SAC_R = ADC_HW_AVG_64X;
				ST7735_OutString("ADC Averaging: 64x");
				break;
			default: 
				break;
		}
		
		record_index = 0;
		memset((void*) numOcurrences, 0, sizeof(int32_t)*NUM_ADC_OUTPUTS);
		EnableInterrupts();
		
		while(1){
			PF1 ^= 0x02;  // toggles when running in main
			if(record_index == MAX_RECORDS){
				DisableInterrupts();
				break;
			}
		}
		
		volatile double jitter = calculate_jitter(time_records, MAX_RECORDS);

		draw_data(ADC_records, MAX_RECORDS);
		
		runNum = (runNum+1) % 4;
		
		Pause();
	}
}


