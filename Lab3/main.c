// filename ******** main.c ************** 
// Trevor Eggenberger - tae498
// Joshua Dong - jid295

#include <stdint.h>
#include <stdio.h>      /* printf, scanf, NULL */
#include <stdlib.h>     /* calloc, exit, free */
#include <string.h>     /* memset */
#include "ADCSWTrigger.h"
#include "../inc/tm4c123gh6pm.h"
#include "PLL.h"
#include "ST7735.h"
#include "fixed.h"
#include "../PeriodicTimer0AInts_4C123/Timer0A.h"
#include "Timer1.h"
#include "../PeriodicTimer2AInts_4C123/Timer2.h"
#include "histogram.h"
#include "speaker.h"
#include "switch.h"
#include "drawLib/draw2D.h"
#include "drawLib/math2.h"

uint16_t COLORS[7] = {
	ST7735_BLUE,
	ST7735_RED,
	ST7735_GREEN,
	ST7735_CYAN,
	ST7735_MAGENTA,
	ST7735_YELLOW,
	ST7735_WHITE
};
typedef enum {ANALOG, DIGITAL} displayMode;
uint32_t secondsElapsed = 0;

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

void UserTask(void){
}

void Timer0A_Handler(void){
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;// acknowledge timer0A timeout
  secondsElapsed = (secondsElapsed+1)%86400;
}

void init_lcd(void) {
  ST7735_InitR(INITR_REDTAB);				//LCD init
}

void init_timers(void){
	Timer0A_Init(&UserTask, 80000000);
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
		Start_Speaker();
  }
  while(PF4==0x10){
    DelayWait10ms(10);
		Stop_Speaker();
  }
}


void drawHands(uint32_t time, point center, int radius, uint16_t color1, uint16_t color2) {
        uint32_t degreesSeconds = (time % 60) * 6;
        uint32_t degreesMinutes = (time % (60 * 60)) / 10;
        uint32_t degreesHours = (time % (60 * 60 * 12) / 120;

	// second hand
	point mEnd = rotPoint(center, degreesSeconds, point_new(center.x, center.y + (radius - 6)));
	drawLine(center, mEnd, color1);
	
        // minute hand
	point mEnd = rotPoint(center, degreesMinutes, point_new(center.x, center.y + (radius - 6)));
	drawLine(center, mEnd, color1);
	
	// hour hand
	point hEnd = rotPoint(center, degreesHours, point_new(center.x, center.y + (radius/2)));
	drawLine(center, hEnd, color2);
}


int main(void){
  init_switches();
  init_lcd();
  init_timers();
  Start_Speaker();
  displayMode = ANALOG;
  
  static int radius = SCREEN_WIDTH * 2 / 5 + 5;
  point center = point_new(SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 16);

  while(1) {
    switch (displayMode) {
      case ANALOG:
        // Clear out previous hands
        drawHands(secondsElapsed - 1, center, radius, 0, 0);
        // Draw new second, minute, hour hands
        drawHands(secondsElapsed, center, radius, COLORS[(i/30) % 7], COLORS[((i/30) + 1) % 7]);
       
        // Redraw analog clock border
        drawCircle(center, radius, COLORS[(i/30)%7]);
        drawCircle(center, radius - 3, COLORS[(i/30)%7]);
        break;
      case DIGITAL:
          break;
      default:
          break;
    }
  }
}


