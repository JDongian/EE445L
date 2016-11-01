#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "../inc/tm4c123gh6pm.h"
#include "switch.h"

#define PD0 (*((volatile uint32_t *)0x40007004))
#define PD1 (*((volatile uint32_t *)0x40007008))

void Init_Switches(void){
	volatile uint32_t delay;
	SYSCTL_RCGCGPIO_R |= 0x20;       // activate port F
  delay = SYSCTL_RCGCGPIO_R;       // allow time to finish activating
	
	// PF3-0 are switch inputs
	GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x0F;           // allow changes to PF3-0
  GPIO_PORTF_AFSEL_R &= ~0x0F;     // disable alt funct on PF3-0
  GPIO_PORTF_DEN_R |= 0x0F;        // enable digital I/O on PF3-0
  GPIO_PORTF_PUR_R |= 0x0F;         // pullup for PF3-0
  GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFF0000)+0x00000000;
  GPIO_PORTF_AMSEL_R = 0;          // disable analog functionality on PF
	GPIO_PORTF_IS_R &= ~0x0F;     // (d) PF4 is edge-sensitive
	GPIO_PORTF_IBE_R |= 0x0F;
	GPIO_PORTF_ICR_R = 0x0F;      // (e) clear flag4
  GPIO_PORTF_IM_R |= 0x0F;      // (f) arm interrupt on PF4 *** No IME bit as mentioned in Book ***
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00400000; // (g) priority 2
  NVIC_EN0_R = 0x40000000;      // (h) enable interrupt 30 in NVIC  
}

void GPIOF_intHandler(){
	GPIO_PORTF_ICR_R = 0x0F;      // (e) clear flag4
}

/*
void Timer0A_Handler(void){
	TIMER0_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER1A timeout
	
		
	static uint16_t pf4State = 0; // Current debounce status
  pf4State=(pf4State<<1) | !PF4 | 0xe000;
	if(pf4State==0xf000){
		playPressed = true;
	}
	
	static uint16_t pf0State = 0; // Current debounce status
  pf0State=(pf0State<<1) | !PF0 | 0xe000;
	if(pf0State==0xf000) {
		changeInstrumentPressed = true;
	}
	
	static uint16_t pb0State = 0; // Current debounce status
  pb0State=(pb0State<<1) | !PB0 | 0xe000;
	if(pb0State==0xf000)rewindPressed = true;
	
}
*/
