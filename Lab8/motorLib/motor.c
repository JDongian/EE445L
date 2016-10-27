#include "motor.h"
#include <stdint.h>

// TODO: correct values
#define PB4 (*((volatile uint32_t *)0x40025008))
#define PB5 (*((volatile uint32_t *)0x40025008))
#define PB6 (*((volatile uint32_t *)0x40025008))
#define PB7 (*((volatile uint32_t *)0x40025008))


struct MotorState state;
// state.left_state = NONE
// state.left_state = NONE
 

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
            PB7 &= 0x00;
            PB6 &= 0x00;
        case FORWARD:
            // right motor forward
            // PB7 = pin15 := HI, PB6 = pin10 := LO
            PB7 |= 0xFF;
            PB6 &= 0x00;
            break;
        case BACKWARD:
            // right motor backward
            // PB7 = pin15 := LO, PB6 = pin10 := HI
            PB7 &= 0x00;
            PB6 |= 0xFF;
            break;
    }
}
