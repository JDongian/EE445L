#include <stdint.h>
#include <stdbool.h>
#include "../inc/tm4c123gh6pm.h"
#include "PLL.h"
#include "Timers.h"
#include "switch.h"

#define PD0 (*((volatile uint32_t *)0x40007004))
#define PD1 (*((volatile uint32_t *)0x40007008))

bool pf0Pressed;
bool pf4Pressed;

void Init_Switches(void){
	volatile uint32_t delay;
	SYSCTL_RCGCGPIO_R |= 0x20;       // activate port F
  delay = SYSCTL_RCGCGPIO_R;       // allow time to finish activating
	
	
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
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00400000; // (g) priority 2
  //NVIC_EN0_R = 0x40000000;      // (h) enable interrupt 30 in NVIC  
	pf0Pressed = false;
	pf4Pressed = false;
	Timer1_Init(180000);          // set up Timer0A for 100 Hz interrupts
}


void Timer1A_Handler(void){
	TIMER0_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER1A timeout
	
		
	static uint16_t pf4State = 0; // Current debounce status
  pf4State=(pf4State<<1) | !PF4 | 0xe000;
	if(pf4State==0xf000){
		pf4Pressed = true;
	}
	
	static uint16_t pf0State = 0; // Current debounce status
  pf0State=(pf0State<<1) | !PF0 | 0xe000;
	if(pf0State==0xf000) {
		pf0Pressed = true;
	}
	
}

