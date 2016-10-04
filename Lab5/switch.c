#include <stdint.h>
#include <stdbool.h>
#include "../inc/tm4c123gh6pm.h"
#include "PLL.h"
#include "Timers.h"
#include "switch.h"

#define PD0 (*((volatile uint32_t *)0x40007004))
#define PD1 (*((volatile uint32_t *)0x40007008))

bool playPressed;

void Init_Switches(void){
	volatile uint32_t delay;
	SYSCTL_RCGCGPIO_R |= 0x20;       // activate port F
  delay = SYSCTL_RCGCGPIO_R;       // allow time to finish activating
  Timer0A_Init(180000);          // set up Timer0A for 100 Hz interrupts
	
	/*
	// PB0 is switch input, PB1 is output to speaker
	GPIO_PORTB_DIR_R |= 0x2;        // make PB1 output (for speaker)
  GPIO_PORTB_AFSEL_R &= ~0x3;     // disable alt funct on PB0, PB1
  GPIO_PORTB_DEN_R |= 0x3;        // enable digital I/O on PB0, PB1
  GPIO_PORTB_AMSEL_R = 0;          // disable analog functionality on PF
	GPIO_PORTB_PUR_R |= 0x1;				// pullup for PB0
	
		// PD0, PD1 is switch input
  GPIO_PORTD_AFSEL_R &= ~0x3;     // disable alt funct on PD0
  GPIO_PORTD_DEN_R |= 0x3;        // enable digital I/O on PD0
  GPIO_PORTD_AMSEL_R = 0;          // disable analog functionality on PF
	GPIO_PORTD_PUR_R |= 0x3;				// pullup for PD0
	*/
	
	// PF0, PF4 are switch inputs
	GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
  GPIO_PORTF_AFSEL_R &= ~0x17;     // disable alt funct on PF2, PF1, PF4
  GPIO_PORTF_DEN_R |= 0x11;        // enable digital I/O on PF2, PF1, PF4
                                   // configure PF2 as GPIO
  GPIO_PORTF_PUR_R |= 0x11;         // pullup for PF0, PF4
  GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFF000F)+0x00000000;
  GPIO_PORTF_AMSEL_R = 0;          // disable analog functionality on PF
	GPIO_PORTF_IS_R &= ~0x11;     // (d) PF4 is edge-sensitive
	GPIO_PORTF_ICR_R = 0x11;      // (e) clear flag4
  //GPIO_PORTF_IM_R |= 0x11;      // (f) arm interrupt on PF4 *** No IME bit as mentioned in Book ***
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00A00000; // (g) priority 5
  //NVIC_EN0_R = 0x40000000;      // (h) enable interrupt 30 in NVIC  
	playPressed = false;
	
}

void Timer0A_Handler(void){
	TIMER0_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER1A timeout
	
		
	static uint16_t pf4State = 0; // Current debounce status
  pf4State=(pf4State<<1) | !PF4 | 0xe000;
	if(pf4State==0xf000){
		playPressed = true;
	}
	
	static uint16_t pf0State = 0; // Current debounce status
  pf0State=(pf0State<<1) | !PF0 | 0xe000;
	//if(pf0State==0xf000) 
	
	/*
  static uint16_t pd0State = 0; // Current debounce status
  pd0State=(pd0State<<1) | !PD0 | 0xe000;
  if(pd0State==0xf000)setAlarmPressed = true; 

	
	static uint16_t pb0State = 0; // Current debounce status
  pb0State=(pb0State<<1) | !PB0 | 0xe000;
	if(pb0State==0xf000)hourChangePressed = true;
	
	static uint32_t pb0Count = 0;
	if (!PB0) pb0Count++;
	else pb0Count = 0;
	if (pb0Count > 400){
		if (pb0Count % 150 == 0){
			hourChangePressed = true;
		}
	}
	
	static uint16_t pd1State = 0; // Current debounce status
	pd1State=(pd1State<<1) | !PD1 | 0xe000;
	if(pd1State==0xf000){
		minuteChangePressed = true;
	}
	
	static uint32_t pd1Count = 0;
	if (!PD1) pd1Count++;
	else pd1Count = 0;
	if (pd1Count > 400){
		if (pd1Count % 150 == 0){
			minuteChangePressed = true;
		}
	}
	*/
}
