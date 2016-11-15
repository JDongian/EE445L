// PWMtest.c
// Runs on TM4C123
// Use PWM0/PB6 and PWM1/PB7 to generate pulse-width modulated outputs.
// Daniel Valvano
// March 28, 2014

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2015
   Program 6.8, section 6.3.2

   "Embedded Systems: Real-Time Operating Systems for ARM Cortex M Microcontrollers",
   ISBN: 978-1466468863, Jonathan Valvano, copyright (c) 2015
   Program 8.4, Section 8.3

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
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "PLL.h"
#include "PWM.h"
#include "ST7735.h"
#include "switch.h"
#include "tach.h"
#include "Motor.h"


void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

int main(void){
	DisableInterrupts();
  PLL_Init(Bus80MHz);               // bus clock at 80 MHz
	Tach_Init();
	Motor_Init();
	Init_Switches();
	ST7735_InitR(INITR_REDTAB);
		ST7735_SetCursor(0,0); 
	ST7735_PlotClear(0,900);  // temperature range from 10 to 40
	ST7735_OutString("Motor Lab");
	ST7735_SetCursor(0,1); 
	ST7735_OutString("rps=");
	ST7735_SetCursor(0,2); 
	ST7735_OutString("desired=");
	uint32_t i = 0;
	EnableInterrupts();
  //PWM0A_Init(40000, 30000);         // initialize PWM0, 1000 Hz, 75% duty
  //PWM0B_Init(40000, 10000);         // initialize PWM0, 1000 Hz, 25% duty
//  PWM0_Duty(4000);    // 10%
//  PWM0_Duty(10000);   // 25%
//  PWM0_Duty(30000);   // 75%

//  PWM0_Init(4000, 2000);         // initialize PWM0, 10000 Hz, 50% duty
//  PWM0_Init(1000, 900);          // initialize PWM0, 40000 Hz, 90% duty
//  PWM0_Init(1000, 100);          // initialize PWM0, 40000 Hz, 10% duty
//  PWM0_Init(40, 20);             // initialize PWM0, 1 MHz, 50% duty
  while(1){
		if (pf4Pressed){
			Motor_IncrementSpeed();
			pf4Pressed = false;
		}
		if (pf0Pressed){
			Motor_DecrementSpeed();
			pf0Pressed = false;
		}
		ST7735_PlotPoints(Tach_GetSpeed(), Motor_GetDesired());  
    if((i&(128-1))==0){          // fs sampling, fs/N samples plotted per second
      ST7735_PlotNextErase();  // overwrites N points on same line
    }
    if((i%10000)==0){    // fs sampling, 1 Hz display of numerical data
      ST7735_SetCursor(5,1); 
			ST7735_OutUDec(Motor_GetActual()/10); 
			ST7735_OutString(".");
			ST7735_OutUDec(Motor_GetActual()%10); 
			ST7735_OutString("   "); // clear previous number
			ST7735_SetCursor(9,2); 
			ST7735_OutUDec(Motor_GetDesired()/10); 
			ST7735_OutString(".");
			ST7735_OutUDec(Motor_GetDesired()%10); 			
			ST7735_OutString(" "); // clear previous number
    }
		i++;
  }
}
