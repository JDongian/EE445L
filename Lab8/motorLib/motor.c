#include <stdint.h>
#include "motor.h"
#include "../../inc/tm4c123gh6pm.h"
#include "../PWM.h"
#include <stdbool.h>

#define PB3 (*((volatile uint32_t *)0x40005020))
#define PB4 (*((volatile uint32_t *)0x40005040))
#define PB5 (*((volatile uint32_t *)0x40005080))
#define PB6 (*((volatile uint32_t *)0x40005100))
#define PB7 (*((volatile uint32_t *)0x40005200))
#define microseconds uint64_t

MotorState left_state;
MotorState right_state;


void motor_init()
{		
		/*
    // initialize PB3-7 (h-bridge interface)
    SYSCTL_RCGCGPIO_R |= 0x02;       // activate port B, D, F
    int delay = SYSCTL_RCGCGPIO_R;   // allow time to finish activating
    GPIO_PORTB_DEN_R |= 0xF8;        // enable digital I/O on PB0, PB1
    GPIO_PORTB_DIR_R |= 0xF8;        // make PB1 output (for speaker)
    GPIO_PORTB_AFSEL_R &= ~0xF8;     // disable alt funct on PB0, PB1
    GPIO_PORTB_AMSEL_R = 0;          // disable analog functionality on PF

    // Make sure nothing is moving to start
    PB4 &= ~(0x01 << 4);
    PB5 &= ~(0x01 << 5);
    PB6 &= ~(0x01 << 6);
    PB7 &= ~(0x01 << 7);
	*/
		PWM0A_Init(10000, 5000);         // initialize PWM0, 4000 Hz, 50% duty
		PWM0B_Init(10000, 2500);         // initialize PWM0, 4000 Hz, 25% duty

    // initialize motor states
    left_state.direction = NONE; // port side
    right_state.direction = NONE; // starboard side
}


MotorState motor_state(Side side)
{
    switch (side) {
        case PORT:
            return left_state;
        case STARBOARD:
				default:
            return right_state;
    }
}


void motor_set(Side side, Dir d, Speed s)
{
    switch (side) {
        case PORT:
            left_state.direction = d;
            left_state.speed = s;
        case STARBOARD:
            right_state.direction = d;
            right_state.speed = s;
    }
}


bool pwm_phase(Speed s, microseconds t)
{
    return true;
    // assert(0 < RESOLUTION <= 1000000)
    // If double math is too slow, change to use integer cutoffs
    return ((t % RESOLUTION) / (double)RESOLUTION) < s;
}


// to be called at least once every microsecond
void motor_run(microseconds time)
{
    // update port side
    bool left_pwm_active = pwm_phase(left_state.speed, time);
    switch (left_state.direction) {
        case NONE:
            // left motor inactive
            //PB4 &= ~(0x01 << 4);
            //PB5 &= ~(0x01 << 5);
						PWM0A_Duty(0);
						PWM0B_Duty(0);
            break;
        case FORWARD:
            // left motor forward
            // PB4 = pin2 := PWM, PB5 = pin7 := LO
            if (left_pwm_active) {
                PB4 |= (0x01 << 4);
            } else {
                PB4 &= ~(0x01 << 4);
            }
            PB5 &= ~(0x01 << 5);
            break;
        case BACKWARD:
            // left motor backward
            // PB4 = pin2 := LO, PB5 = pin7 := PWM
            if (left_pwm_active) {
                PB5 |= (0x01 << 5);
            } else {
                PB5 &= ~(0x01 << 5);
            }
            PB4 &= ~(0x01 << 4);
            break;
    }

    // update starboard side
    bool right_pwm_active = pwm_phase(right_state.speed, time);
    switch (right_state.direction) {
        case NONE:
            // right motor inactive
            PB6 &= ~(0x01 << 6);
            PB7 &= ~(0x01 << 7);
            break;
        case FORWARD:
            // right motor forward
            // PB7 = pin15 := PWM, PB6 = pin10 := LO
            if (right_pwm_active) {
                PB7 |= (0x01 << 7);
            } else {
                PB7 &= ~(0x01 << 7);
            }
            PB6 &= ~(0x01 << 6);
            break;
        case BACKWARD:
            // right motor backward
            // PB7 = pin15 := LO, PB6 = pin10 := PWM
            if (right_pwm_active) {
                PB6 |= (0x01 << 6);
            } else {
                PB6 &= ~(0x01 << 6);
            }
            PB7 &= ~(0x01 << 7);
            break;
    }
}
