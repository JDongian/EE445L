// MAX5353TestMain.c
// Runs on LM4F120/TM4C123
// Test the functions provided in MAX5353.c by outputting
// a sine wave at a particular frequency.
// Daniel Valvano
// September 11, 2013

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2014

 Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu
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
#include "Song.h"

// DEBUG LED
#define PF1       (*((volatile uint32_t *)0x40025008))
	
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

void Init_LED(){
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

    // initialize hardware
    DAC_Init();                
    Init_Switches();
    Init_LED();

    // initialize timers used for music playing
    Timer1_Init(1);
    Timer2_Init(1);
    Timer3_Init(1);
    Timer4_Init(1);
    // an arbitrarily small value
    Timer5_Init(2000);

    SysTick_Init();

    // initialize song data
    init_songs();

    // play a song with two lines
    PlayMusic(&guiles_melody, &guiles_tuba);

    EnableInterrupts();

    while(1){
        // handle hardware buttons
        if (playPressed){
            TIMER1_CTL_R ^= 1;
            TIMER2_CTL_R ^= 1;
            TIMER3_CTL_R ^= 1;
            TIMER4_CTL_R ^= 1;
            playPressed = false;
        }
        if (changeInstrumentPressed) {
            ChangeInstrument();
            changeInstrumentPressed = false;
        }
    }
}
