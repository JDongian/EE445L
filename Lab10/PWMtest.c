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

#define PLOT_SPEED 700
#define TEXT_SPEED 10000


void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

uint32_t i = 0;

void handle_switches(){
	if (pf4Pressed){
			Motor_IncrementSpeed();
			pf4Pressed = false;
		}
	if (pf0Pressed){
		Motor_DecrementSpeed();
		pf0Pressed = false;
	}
}

void init_motorGraph(){
	ST7735_SetCursor(0,0); 
	ST7735_PlotClear(0,900);  // motor range 0 to 90 rps
	ST7735_OutString("Motor Lab");
	ST7735_SetCursor(0,1); 
	ST7735_OutString("Set=");
	ST7735_SetCursor(0,2); 
	ST7735_OutString("Measured=");
}

void update_motorGraph(){
		
		if ((i % PLOT_SPEED) == 0){
			ST7735_PlotPoints(Motor_GetActual(), Motor_GetDesired());  
		}
    if((i%((ST7735_TFTWIDTH-1)*PLOT_SPEED))==0){          
      ST7735_PlotNextErase();  // overwrites 127 points (pixels) on same line
    }
    if((i%TEXT_SPEED)==0){   
      ST7735_SetCursor(10,2); 
			// print measured motor speed
			ST7735_OutUDec(Motor_GetActual()/10);  // integer part
			ST7735_OutString(".");
			ST7735_OutUDec(Motor_GetActual()%10); 	// decimal part
			ST7735_OutString(" rps  "); 
			// print desired motor speed
			ST7735_SetCursor(5,1); 
			ST7735_OutUDec(Motor_GetDesired()/10); // integer part
			ST7735_OutString(".");
			ST7735_OutUDec(Motor_GetDesired()%10); // decimal part			
			ST7735_OutString(" rps  "); 
    }
		i++;
}

int main(void){
	DisableInterrupts();
  PLL_Init(Bus80MHz); // bus clock at 80 MHz
	Tach_Init();	// initialize inpu capture
	Motor_Init();	// initialize pwm
	Init_Switches();	// initialize pf0 and pf4 switches
	ST7735_InitR(INITR_REDTAB);
	init_motorGraph();	// initialize LCD plot
	i = 0;
	EnableInterrupts();
  while(1){
		handle_switches();
		update_motorGraph();
  }
}
