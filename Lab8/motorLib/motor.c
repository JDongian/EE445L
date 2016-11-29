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
    // initialize PB3-7 (h-bridge interface)
		PWM0A_Init(40000, 0);
		PWM0B_Init(40000, 0);
		PWM1A_Init(40000, 0);
		PWM1B_Init(40000, 0);
    //SYSCTL_RCGCGPIO_R |= 0x02;       // activate port B
    //int delay = SYSCTL_RCGCGPIO_R;   // allow time to finish activating
    GPIO_PORTB_DEN_R |= 0x08;        // enable digital I/O on PB0, PB1
    GPIO_PORTB_DIR_R |= 0x08;        // make PB1 output (for speaker)
    GPIO_PORTB_AFSEL_R &= ~0x08;     // disable alt funct on PB0, PB1
    GPIO_PORTB_AMSEL_R = 0;          // disable analog functionality on PF
		

    // Make sure nothing is moving to start
		PB3 = 0x08;
    //PB4 &= ~(0x01 << 4);
    //PB5 &= ~(0x01 << 5);
    //PB6 &= ~(0x01 << 6);
    //PB7 &= ~(0x01 << 7);

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


void motor_set(Side side, Dir d, int speed)
{
    switch (side) {
        case PORT:
            left_state.direction = d;
            left_state.speed = speed;
        case STARBOARD:
            right_state.direction = d;
            right_state.speed = speed;
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
void motor_run()
{
    // update port side
    switch (left_state.direction) {
        case NONE:
            // left motor inactive
            PWM1A_Duty(0);
						PWM1B_Duty(0);
            break;
        case FORWARD:
            // left motor forward
            // PB4 = pin2 := PWM, PB5 = pin7 := LO
            PWM1A_Duty(left_state.speed);
						PWM1B_Duty(0);
            break;
        case BACKWARD:
            // left motor backward
            // PB4 = pin2 := LO, PB5 = pin7 := PWM
            PWM1A_Duty(0);
						PWM1B_Duty(left_state.speed);
            break;
    }

    // update starboard side
    switch (right_state.direction) {
        case NONE:
            // right motor inactive
            PWM0A_Duty(0);
						PWM0B_Duty(0);
            break;
        case FORWARD:
            // right motor forward
            // PB7 = pin15 := PWM, PB6 = pin10 := LO
            PWM0A_Duty(right_state.speed);
						PWM0B_Duty(0);
            break;
        case BACKWARD:
            // right motor backward
            // PB7 = pin15 := LO, PB6 = pin10 := PWM
            PWM0A_Duty(0);
						PWM0B_Duty(right_state.speed);
            break;
    }
}

void motor_off()
{
	PB3 = 0;
}
