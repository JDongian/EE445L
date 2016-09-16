#ifndef _SW_PORTS
#define _SW_PORTS
#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
#define PF4   		(*((volatile uint32_t *)0x40025040))
	
#define PB1 			(*((volatile uint32_t *)0x40005008))
#endif /* _SW_PORTS */
	
void init_switches(void);
