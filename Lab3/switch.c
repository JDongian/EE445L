#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "PLL.h"
#include "../PeriodicTimer0AInts_4C123/Timer0A.h"
#include "switch.h"

void SpeakerTask(void){
	PB1 ^= 0x2;
}

void init_switches(void){
	volatile uint32_t delay;
	
	PLL_Init(Bus80MHz);              // bus clock at 80 MHz
	SYSCTL_RCGCGPIO_R |= 0x2A;       // activate port B, D, F
  delay = SYSCTL_RCGCGPIO_R;       // allow time to finish activating
  //ADC0_InitSWTriggerSeq3_Ch9();    // allow time to finish activating
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
	
  //Timer1_Init(&UserTask, 0xFFFFFFFF); // initialize timer1 (slowest rate)
	//Timer2_Init(&UserTask2, 7920); // initialize timer2 to ~99 us period
	
  EnableInterrupts();
}