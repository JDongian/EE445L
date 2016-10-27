#include "motor.h"
#include "../../inc/tm4c123gh6pm.h"
#include <stdint.h>

// TODO: correct values
#define PB3 (*((volatile uint32_t *)0x40005020))
#define PB4 (*((volatile uint32_t *)0x40005040))
#define PB5 (*((volatile uint32_t *)0x40005080))
#define PB6 (*((volatile uint32_t *)0x40005100))
#define PB7 (*((volatile uint32_t *)0x40005200))


struct MotorState state;
// state.left_state = NONE
// state.left_state = NONE

// initialize PB3-7
void motor_init() {
	SYSCTL_RCGCGPIO_R |= 0x02;       // activate port B, D, F
  int delay = SYSCTL_RCGCGPIO_R;       // allow time to finish activating
	 GPIO_PORTB_DEN_R |= 0xF8;        // enable digital I/O on PB0, PB1
	GPIO_PORTB_DIR_R |= 0xF8;        // make PB1 output (for speaker)
  GPIO_PORTB_AFSEL_R &= ~0xF8;     // disable alt funct on PB0, PB1
  GPIO_PORTB_AMSEL_R = 0;          // disable analog functionality on PF
}

void motor_set(Side side, Dir direction)
{
    switch (side) {
        case PORT:
            state.left_state = direction;
        case STARBOARD:
            state.right_state = direction;
    }
}


void motor_run()
{
    // update port side
    switch (state.left_state) {
        case NONE:
            // left motor inactive
            PB4 &= 0x00;
            PB5 &= 0x00;
						break;
        case FORWARD:
            // left motor forward
            // PB4 = pin2 := HI, PB5 = pin7 := LO
            PB4 |= 0xFF;
            PB5 &= 0x00;
            break;
        case BACKWARD:
            // left motor backward
            // PB4 = pin2 := LO, PB5 = pin7 := HI
            PB4 &= 0x00;
            PB5 |= 0xFF;
            break;
    }

    // update starboard side
    switch (state.right_state) {
        case NONE:
            // right motor inactive
            PB7 &= ~(0x80);
            PB6 &= ~(0x40);
				    break;
        case FORWARD:
            // right motor forward
            // PB7 = pin15 := HI, PB6 = pin10 := LO
            PB7 |= 0x80;
            PB6 &= ~(0x40);
            break;
        case BACKWARD:
            // right motor backward
            // PB7 = pin15 := LO, PB6 = pin10 := HI
            PB7 &= ~(0x80);
            PB6 |= 0x40;
            break;
    }
}
