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

typedef struct timeStamp {
	uint8_t hours;
	uint8_t minutes;
} timeStamp;

timeStamp alarmTime;
bool alarmOn = false;
bool alarmSet = false;

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
	alarmTime.hours = 0;
	alarmTime.minutes = 0;
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


void drawHands(uint32_t time, point center, int radius, bool reset) {
				//time *= -1;
        int32_t degreesSeconds = (time % 60) * -6;
        int32_t degreesMinutes = -1*(time % (60 * 60) / 10); 
        int32_t degreesHours = -1*((time % (60 * 60 * 12)) / 120);

	// second hand
	point sEnd = rotPoint(center, degreesSeconds, point_new(center.x, center.y + (radius - 4)));
	drawLine(center, sEnd, reset? 0 : ST7735_RED);
	
        // minute hand
	point mEnd = rotPoint(center, degreesMinutes, point_new(center.x, center.y + (radius - 10)));
	drawLine(center, mEnd, reset? 0 : ST7735_BLUE);
	
	// hour hand
	point hEnd = rotPoint(center, degreesHours, point_new(center.x, center.y + (radius/2)));
	drawLine(center, hEnd, reset? 0 : ST7735_WHITE);
}

void drawHourHand(uint32_t time, point center, int radius, bool reset){
	int32_t degreesHours = -1*((time % (60 * 60 * 12)) / 120);
	point hEnd = rotPoint(center, degreesHours, point_new(center.x, center.y + (radius/2)));
	drawLine(center, hEnd, reset? 0 : ST7735_WHITE);
}

void drawMinuteHand(uint32_t time, point center, int radius, bool reset) {
	int32_t degreesMinutes = -1*(time % (60 * 60) / 10); 
	point mEnd = rotPoint(center, degreesMinutes, point_new(center.x, center.y + (radius - 10)));
	drawLine(center, mEnd, reset? 0 : ST7735_BLUE);
}

void drawDigital(uint32_t time){
	uint8_t hours = (time / 3600) % 24;
	uint8_t minutes = (time  / 60) % 60;
	uint8_t seconds = time % 60;
	
	ST7735_DrawCharS(18, 65, hours/10 + '0', ST7735_Color565(255, 0, 0), 0, 2);
  ST7735_DrawCharS(30, 65, hours%10 + '0', ST7735_Color565(255, 0, 0), 0, 2);
  ST7735_DrawCharS(42, 65, ':', ST7735_Color565(255, 255, 255), 0, 2);
  ST7735_DrawCharS(54, 65, minutes/10 + '0', ST7735_Color565(0, 255, 0), 0, 2);
  ST7735_DrawCharS(66, 65, minutes%10 + '0', ST7735_Color565(0, 255, 0), 0, 2);
	ST7735_DrawCharS(78, 65, ':', ST7735_Color565(255, 255, 255), 0, 2);
  ST7735_DrawCharS(90, 65, seconds/10 + '0', ST7735_Color565(0, 255, 255), 0, 2);
  ST7735_DrawCharS(102, 65, seconds%10 + '0', ST7735_Color565(0, 255, 255), 0, 2);
}

void drawString(int16_t x, int16_t y, char* s, int16_t textColor, int16_t bgColor, uint8_t textSize, uint16_t n){
	for (int i = 0; i < n; i++){
		ST7735_DrawCharS(6*i+x, y, s[i], textColor, bgColor, textSize);
	}
}

void displayAlarmStatus(bool setAlarm) {
	char displayText[] = "Setting";
	char alarmText[] = "Alarm";
	char timeText[] = "Time ";
	char on[] = "on ";
	char off[] = "off";
	
	drawString(10, 130, displayText, ST7735_WHITE, 0, 1, 7);
	
	if (setAlarm){
		drawString(58, 130, alarmText, ST7735_WHITE, 0, 1, 5);
	}
	else {
		drawString(58, 130, timeText, ST7735_WHITE, 0, 1, 5);
	}
	
	char buffer[6];
	if (setAlarm) {
		sprintf(buffer, "%02d:%02d", alarmTime.hours, alarmTime.minutes);
		drawString(10, 140, buffer, ST7735_WHITE, 0, 1, 5);
	}
	else {
		sprintf(buffer, "%02d:%02d", (secondsElapsed / 3600) % 24, (secondsElapsed / 60) % 60);
		drawString(10, 140, buffer, ST7735_WHITE, 0, 1, 5);
	}

	drawString(10, 150, alarmText, ST7735_WHITE, 0, 1, 5);
	
	if (alarmSet){
		drawString(46, 150, on, ST7735_WHITE, 0, 1, 3);
	}
	else {
		drawString(46, 150, off, ST7735_WHITE, 0, 1, 3);
	}
	
}

bool timeEquals(uint32_t t1, uint32_t t2) {
	return t1 == t2;
}

int main(void){
	bool settingAlarm = false;
  init_switches();
  init_lcd();
  init_timers();
  displayMode mode = ANALOG;
	  
  static int radius = SCREEN_WIDTH * 2 / 5 + 5;
  point center = point_new(SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 16);

  while(1) {
		if (alarmOn) {
			Start_Speaker();
		}
		else Stop_Speaker();
		
		if (alarmOnPressed){
			if (alarmOn) alarmOn = false;
			alarmSet ^= 1;
			alarmOnPressed = false;
		}
		
		if (changeModePressed) {
			changeModePressed = false;
			mode ^= 1;
			ST7735_FillScreen(0);
		}
		
		if (setAlarmPressed){
			setAlarmPressed = false;
			settingAlarm ^= 1;
		}
		
    switch (mode) {
      case ANALOG:
        // Clear out previous hands
        drawHands(secondsElapsed - 1, center, radius, true);
        // Draw new second, minute, hour hands
        drawHands(secondsElapsed, center, radius, false);
       
        // Redraw analog clock border
        drawCircle(center, radius, COLORS[(secondsElapsed/30)%7]);
        drawCircle(center, radius - 3, COLORS[(secondsElapsed/30)%7]);
        break;
      case DIGITAL:
					drawDigital(secondsElapsed);
          break;
      default:
          break;
    }
		if (hourChangePressed) {
			if (settingAlarm){
				alarmTime.hours = (alarmTime.hours + 1) % 24;
			}
			else {
				drawHourHand(secondsElapsed, center, radius, true);
				secondsElapsed += 3600;
			}
			hourChangePressed = false;
		}
		if (minuteChangePressed) {
			if (settingAlarm){
				alarmTime.minutes = (alarmTime.minutes + 1) % 60;
			}
			else {
				drawHourHand(secondsElapsed, center, radius, true);
				drawMinuteHand(secondsElapsed, center, radius, true);
				secondsElapsed += 60;
			}
			minuteChangePressed = false;
		}
		if (timeEquals(alarmTime.hours*3600 + alarmTime.minutes*60, secondsElapsed) && alarmSet){
			alarmOn = true;
		}
		displayAlarmStatus(settingAlarm);
  }
}


