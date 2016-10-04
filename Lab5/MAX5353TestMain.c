// MAX5353TestMain.c
// Runs on LM4F120/TM4C123

// SSI0Clk (SCLK, pin 4) connected to PA2
// SSI0Fss (!CS, pin 2) connected to PA3
// SSI0Tx (DIN, pin 3) connected to PA5
// see Figure 7.19 for complete schematic

#include <stdint.h>
#include "MAX5353.h"
#include "SysTick.h"
#include "Timers.h"
#include "PLL.h"
#include "../inc/tm4c123gh6pm.h"
#include "Music.h"
#include "Wave.h"
#include "switch.h"

#define PF1       (*((volatile uint32_t *)0x40025008))
	

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode


void init_led()
{
  GPIO_PORTF_DIR_R |= 0x6;        // make PF2, PF1 out (built-in LED)
  GPIO_PORTF_AFSEL_R &= ~0x16;     // disable alt funct on PF2, PF1, PF4
  GPIO_PORTF_DEN_R |= 0x16;        // enable digital I/O on PF2, PF1, PF4
                                   // configure PF2 as GPIO
  GPIO_PORTF_PUR_R |= 0x10;         // pullup for PF4
  GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFF000F)+0x00000000;
  GPIO_PORTF_AMSEL_R = 0;          // disable analog functionality on PF
}


int main(void)
{
  PLL_Init(Bus80MHz);

  DisableInterrupts();

  // Initialize hardware
  init_dac();
  init_switches();
  init_led();

  // somewhat arbitrary initial times
  // Two timers handle sound
  Timer1_Init(80000000/440/32);
  Timer2_Init(1);

  SysTick_Init();
  EnableInterrupts();

  while(1) {
    if (playPressed) {
      TIMER1_CTL_R ^= 1;
      TIMER2_CTL_R ^= 1;
      playPressed = false;
    }
  }
}
