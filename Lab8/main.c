/* main.c
 * Trevor Eggenberger - tae498
 * Joshua Dong - jid295
 */

#include <stdint.h>
//#include <stdio.h>      /* printf, scanf, NULL */
//#include <stdlib.h>     /* calloc, exit, free */
//#include <string.h>     /* memset */

//#include "ADCSWTrigger.h"
#include "../inc/tm4c123gh6pm.h"
#include "PLL.h"
//#include "ST7735.h"
#include "fixed.h"
//#include "Timers.h"
#include "motorLib/motor.h"


void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode


int main(void){
  //Init_Timers();
  motor_set(PORT, FORWARD);
  motor_set(STARBOARD, FORWARD);
	  
  while(1) {
    //handleButtons();

    //switch (mode) {
    //} 
  }
}


