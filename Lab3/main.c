// filename ******** main.c ************** 
// Trevor Eggenberger - tae498
// Joshua Dong - jid295


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
#include <stdio.h>      /* printf, scanf, NULL */
#include <stdlib.h>     /* calloc, exit, free */
#include <string.h>     /* memset */
#include "ADCSWTrigger.h"
#include "../inc/tm4c123gh6pm.h"
#include "PLL.h"
#include "ST7735.h"
#include "fixed.h"
#include "../PeriodicTimer0AInts_4C123/Timer0A.h"
#include "Timer1.h"
#include "../PeriodicTimer2AInts_4C123/Timer2.h"
#include "histogram.h"
#include "speaker.h"

#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
#define PF4   		(*((volatile uint32_t *)0x40025040))
	
#define PB1 			(*((volatile uint32_t *)0x40005008))
	

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode


void SpeakerTask(void){
	PB1 ^= 0x2;
}

void UserTask2(void){}


void init_hw(void){
	volatile uint32_t delay;
	
	PLL_Init(Bus80MHz);              // bus clock at 80 MHz
	SYSCTL_RCGCGPIO_R |= 0x2A;       // activate port B, D, F
  delay = SYSCTL_RCGCGPIO_R;       // allow time to finish activating
  ADC0_InitSWTriggerSeq3_Ch9();    // allow time to finish activating
  Timer0A_Init(&SpeakerTask, 7999999);          // set up Timer0A for 100 Hz interrupts
	
	// PB0 is switch input, PB1 is output to speaker
	GPIO_PORTB_DIR_R |= 0x2;        // make PB1 output (for speaker)
  GPIO_PORTB_AFSEL_R &= ~0x3;     // disable alt funct on PB0, PB1
  GPIO_PORTB_DEN_R |= 0x3;        // enable digital I/O on PB0, PB1
  GPIO_PORTB_AMSEL_R = 0;          // disable analog functionality on PF
	GPIO_PORTB_PUR_R |= 0x1;				// pullup for PB0
	
	// PD0 is switch input
  GPIO_PORTD_AFSEL_R &= ~0x1;     // disable alt funct on PB0
  GPIO_PORTD_DEN_R |= 0x1;        // enable digital I/O on PB0
  GPIO_PORTD_AMSEL_R = 0;          // disable analog functionality on PF
	GPIO_PORTD_PUR_R |= 0x1;				// pullup for PD0
	
  GPIO_PORTF_DIR_R |= 0x6;        // make PF2, PF1 out (built-in LED)
  GPIO_PORTF_AFSEL_R &= ~0x16;     // disable alt funct on PF2, PF1, PF4
  GPIO_PORTF_DEN_R |= 0x16;        // enable digital I/O on PF2, PF1, PF4
                                   // configure PF2 as GPIO
  GPIO_PORTF_PUR_R |= 0x10;         // pullup for PF4
  GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFF000F)+0x00000000;
  GPIO_PORTF_AMSEL_R = 0;          // disable analog functionality on PF
	
  ST7735_InitR(INITR_REDTAB);				//LCD init
	
  //Timer1_Init(&UserTask, 0xFFFFFFFF); // initialize timer1 (slowest rate)
	//Timer2_Init(&UserTask2, 7920); // initialize timer2 to ~99 us period

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
		Start_Speaker();
  }
  while(PF4==0x10){
    DelayWait10ms(10);
		Stop_Speaker();
  }
}

int main(void){
  init_hw();
	Start_Speaker();
 
  while(1) {
		Pause();
  }
}


