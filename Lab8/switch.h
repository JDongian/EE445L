#ifndef _SW_PORTS
#define _SW_PORTS

#define PB0 			(*((volatile uint32_t *)0x40005004))
#define PB1 			(*((volatile uint32_t *)0x40005008))
#endif /* _SW_PORTS */

#include <stdbool.h>

extern bool sw1;	
extern bool sw2;
extern bool sw3;
extern bool sw4;

void Init_Switches(void);
