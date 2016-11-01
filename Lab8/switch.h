#ifndef _SW_PORTS
#define _SW_PORTS
#define PF0				(*((volatile uint32_t *)0x40025004))
#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
#define PF4   		(*((volatile uint32_t *)0x40025040))

#define PB0 			(*((volatile uint32_t *)0x40005004))
#define PB1 			(*((volatile uint32_t *)0x40005008))
#endif /* _SW_PORTS */

#include <stdbool.h>

extern bool playPressed;	
extern bool changeInstrumentPressed;
extern bool rewindPressed;

void Init_Switches(void);
