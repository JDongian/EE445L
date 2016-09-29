// filename ******** Timers.h ************** 
// Trevor Eggenberger - tae498
// Joshua Dong - jid295

#include <stdint.h>

void Timer0A_Init(void(*task)(void), uint32_t period);

void Timer1_Init(void(*task)(void), uint32_t period);
	
void Timer2_Init(void(*task)(void), uint32_t period);
