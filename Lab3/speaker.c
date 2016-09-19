// filename ******** speaker.c ************** 
// Trevor Eggenberger - tae498
// Joshua Dong - jid295

#include <stdint.h>
#include "..//inc//tm4c123gh6pm.h"

void Start_Speaker(void) {
	TIMER2_CTL_R = 0x00000001;    // 10) enable TIMER0A
}

void Stop_Speaker(void) {
	TIMER2_CTL_R = 0x00000000;    // 10) disable TIMER0A
}
