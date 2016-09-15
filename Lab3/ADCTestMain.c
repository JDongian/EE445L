// ADCTestMain.c

#include <stdint.h>
#include "ADCSWTrigger.h"
#include "../inc/tm4c123gh6pm.h"
#include "PLL.h"
#include "ST7735.h"
#include "fixed.h"
#include "Timer1.h"
#include "../PeriodicTimer2AInts_4C123/Timer2.h"
#include "histogram.h"
#include <stdio.h>      /* printf, scanf, NULL */
#include <stdlib.h>     /* calloc, exit, free */
#include <string.h>     /* memset */
#include "drawLib/draw2D.h"
#include "drawLib/math2.h"

#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
#define LEDS      (*((volatile uint32_t *)0x40025038))
#define PF4   (*((volatile uint32_t *)0x40025040))

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode


// This debug function initializes Timer0A to request interrupts
// at a 100 Hz frequency.  It is similar to FreqMeasure.c.
void Timer0A_Init100HzInt(void){
  volatile uint32_t delay;
  DisableInterrupts();
  // **** general initialization ****
  SYSCTL_RCGCTIMER_R |= 0x01;      // activate timer0
  delay = SYSCTL_RCGCTIMER_R;      // allow time to finish activating
  TIMER0_CTL_R &= ~TIMER_CTL_TAEN; // disable timer0A during setup
  TIMER0_CFG_R = 0;                // configure for 32-bit timer mode
  // **** timer0A initialization ****
                                   // configure for periodic mode
  TIMER0_TAMR_R = TIMER_TAMR_TAMR_PERIOD;
  TIMER0_TAILR_R = 799999*16;         // start value for 100 Hz interrupts
  TIMER0_IMR_R |= TIMER_IMR_TATOIM;// enable timeout (rollover) interrupt
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;// clear timer0A timeout flag
  TIMER0_CTL_R |= TIMER_CTL_TAEN;  // enable timer0A 32-b, periodic, interrupts
  // **** interrupt initialization ****
                                   // Timer0A=priority 2
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x40000000; // top 3 bits
  NVIC_EN0_R = 1<<19;              // enable interrupt 19 in NVIC
}
void Timer0A_Handler(void){
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;    // acknowledge timer0A timeout
  PF2 ^= 0x04;                   // profile
}


void UserTask(void){
}

void init_lcd(void) {
  ST7735_InitR(INITR_REDTAB);				//LCD init
}
void init_ports(void) {
	volatile uint32_t delay;
	
	PLL_Init(Bus80MHz);              // bus clock at 80 MHz
	SYSCTL_RCGCGPIO_R |= 0x20;       // activate port F
  delay = SYSCTL_RCGCGPIO_R;       // allow time to finish activating?
  ADC0_InitSWTriggerSeq3_Ch9();    // allow time to finish activating
  Timer0A_Init100HzInt();          // set up Timer0A for 100 Hz interrupts
  GPIO_PORTF_DIR_R |= 0x6;         // make PF2, PF1 out (built-in LED)
  GPIO_PORTF_AFSEL_R &= ~0x16;     // disable alt funct on PF2, PF1, PF4
  GPIO_PORTF_DEN_R |= 0x16;        // enable digital I/O on PF2, PF1, PF4
                                   // configure PF2 as GPIO
  GPIO_PORTF_PUR_R |= 0x10;         // pullup for PF4
  GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFF000F)+0x00000000;
  GPIO_PORTF_AMSEL_R = 0;          // disable analog functionality on PF
  LEDS = 0;                        // turn LEDs off
}
	
void init_timers(void){
  Timer1_Init(&UserTask, 0xFFFFFFFF); // initialize timer1 (slowest rate)
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
  }
  while(PF4==0x10){
    DelayWait10ms(10);
  }
}


int main(void){
	uint16_t colors[7] = {
		ST7735_BLUE,
		ST7735_RED,
		ST7735_GREEN,
		ST7735_CYAN,
		ST7735_MAGENTA,
		ST7735_YELLOW,
		ST7735_WHITE
  };
	
	init_ports();
  init_lcd();
	init_timers();
	
	initBuffer();
	
	int radius = SCREEN_WIDTH * 2 / 5 + 5;
	point center = point_new(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
  while(1) {
      //EnableInterrupts();
/*
      drawPoint(1, 1, ST7735_MAGENTA);

      drawLine(point_new(40,40), point_new(40,70), ST7735_RED);
      drawLine(point_new(70,40), point_new(70,70), ST7735_RED);

      finishBuffer();
			attemptRender();
		*/
      /*
			ST7735_drawLine(point_new(40,40), point_new(40,70), ST7735_RED);
			ST7735_drawLine(point_new(70,40), point_new(70,70), ST7735_RED);
			ST7735_drawLine(point_new(20,80), point_new(40,90), ST7735_RED);
			ST7735_drawLine(point_new(40,90), point_new(70,90), ST7735_RED);
			ST7735_drawLine(point_new(70,90), point_new(90,80), ST7735_RED);
		*/
	
      int i = 0;
      while (1) {
				 
        i += 2;
				
        clearBuffer();
        //drawLine(point_new(1,1), point_new(i+i/3,i), colors[i%7]);
				
				drawCircle(center, radius, colors[i%7]);
				drawCircle(center, radius - 3, colors[i%7]);
				
				// minute hand
				point mEnd = rotPoint(center, (-1 * i) % 360, point_new(center.x, center.y + (radius - 6)));
				drawLine(center, mEnd, colors[(i+1)%7]);
				
				// hour hand
				point hEnd = rotPoint(center, (-1 * i / 5) % 360, point_new(center.x, center.y + (radius/2)));
				drawLine(center, hEnd, colors[(i+2)%7]);
				
				
        finishBuffer();
				attemptRender();
      }
      //DisableInterrupts();
  }
}


