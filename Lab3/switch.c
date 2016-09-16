#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "PLL.h"
#include "../PeriodicTimer0AInts_4C123/Timer0A.h"
#include "switch.h"

#define PD0 (*((volatile uint32_t *)0x40007004))

int count;
volatile static unsigned long Last;      // previous

void SpeakerTask(void){
	PB1 ^= 0x2;
}

void DelayWait10ms2(uint32_t n){
	uint32_t volatile time;
  while(n){
    time = 727240*2/91;  // 10msec
    while(time){
	  	time--;
    }
    n--;
  }
}

static void Timer1Arm(uint32_t period){      
  TIMER1_CTL_R = 0x00000000;    // 1) disable TIMER1A during setup
  TIMER1_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER1_TAMR_R = 0x00000002;   // 3) configure for one-shot mode
  TIMER1_TAILR_R = period-1;    // 4) reload value
  TIMER1_TAPR_R = 0;            // 5) bus clock resolution
  TIMER1_ICR_R = 0x00000001;    // 6) clear TIMER1A timeout flag
  TIMER1_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0xFFFF00FF)|0x00008000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 37, interrupt number 21
  NVIC_EN0_R = 1<<21;           // 9) enable IRQ 21 in NVIC
  TIMER1_CTL_R = 0x00000001;    // 10) enable TIMER1A
}

void init_switches(void){
	volatile uint32_t delay;
	
	PLL_Init(Bus80MHz);              // bus clock at 80 MHz
	SYSCTL_RCGCGPIO_R |= 0x2A;       // activate port B, D, F
  delay = SYSCTL_RCGCGPIO_R;       // allow time to finish activating
  //ADC0_InitSWTriggerSeq3_Ch9();    // allow time to finish activating
  Timer0A_Init(&SpeakerTask, 180000);          // set up Timer0A for 100 Hz interrupts
	
	// PB0 is switch input, PB1 is output to speaker
	GPIO_PORTB_DIR_R |= 0x2;        // make PB1 output (for speaker)
  GPIO_PORTB_AFSEL_R &= ~0x3;     // disable alt funct on PB0, PB1
  GPIO_PORTB_DEN_R |= 0x3;        // enable digital I/O on PB0, PB1
  GPIO_PORTB_AMSEL_R = 0;          // disable analog functionality on PF
	GPIO_PORTB_PUR_R |= 0x1;				// pullup for PB0
	
  GPIO_PORTF_DIR_R |= 0x6;        // make PF2, PF1 out (built-in LED)
  GPIO_PORTF_AFSEL_R &= ~0x16;     // disable alt funct on PF2, PF1, PF4
  GPIO_PORTF_DEN_R |= 0x16;        // enable digital I/O on PF2, PF1, PF4
                                   // configure PF2 as GPIO
  GPIO_PORTF_PUR_R |= 0x10;         // pullup for PF4
  GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFF000F)+0x00000000;
  GPIO_PORTF_AMSEL_R = 0;          // disable analog functionality on PF
	GPIO_PORTF_IS_R &= ~0x10;     // (d) PF4 is edge-sensitive
	GPIO_PORTF_ICR_R = 0x10;      // (e) clear flag4
  GPIO_PORTF_IM_R |= 0x10;      // (f) arm interrupt on PF4 *** No IME bit as mentioned in Book ***
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00A00000; // (g) priority 5
  NVIC_EN0_R = 0x40000000;      // (h) enable interrupt 30 in NVIC  
	
		
	// PD0 is switch input
  GPIO_PORTD_AFSEL_R &= ~0x1;     // disable alt funct on PD0
  GPIO_PORTD_DEN_R |= 0x1;        // enable digital I/O on PD0
  GPIO_PORTD_AMSEL_R = 0;          // disable analog functionality on PF
	GPIO_PORTD_PUR_R |= 0x1;				// pullup for PD0
	GPIO_PORTD_IS_R &= ~0x1;     // (d) PF4 is edge-sensitive
	GPIO_PORTD_ICR_R = 0x1;      // (e) clear flag4
  //GPIO_PORTD_IM_R |= 0x1;      // (f) arm interrupt on PD0 *** No IME bit as mentioned in Book ***
	NVIC_PRI0_R = (NVIC_PRI0_R&0x0FFFFFFF)|0xA0000000; // priority 5
  //NVIC_EN0_R = 0x00000008;      // (h) enable interrupt 3 in NVIC  
	
	SYSCTL_RCGCTIMER_R |= 0x02;   // 0) activate TIMER1   
	Timer1Arm(50000);
}


static void GPIOArm(void){
  GPIO_PORTD_ICR_R = 0x1;      // (e) clear flag4
  GPIO_PORTD_IM_R |= 0x1;      // (f) arm interrupt on PF4 *** No IME bit as mentioned in Book ***
}

void GPIOPortD_Handler(void) {
	GPIO_PORTD_IM_R &= ~0x1;     // disarm interrupt on PF4 
	if (!Last) count++;
	Timer1Arm(800000);
}

void GPIOPortF_Handler(void){
	GPIO_PORTF_ICR_R = 0x10;      // (e) clear flag4
	count++;
}

void Timer1A_Handler(void){
	//TIMER1_ICR_R = 0x00000000;    // disarm timeout interrupt
	TIMER1_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER1A timeout
  static uint16_t State = 0; // Current debounce status
  State=(State<<1) | !PD0 | 0xe000;
  if(State==0xf000)count++; 
}
