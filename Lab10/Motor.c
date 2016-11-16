#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "tach.h"
#include "PWM.h"
#include "Timers.h"

#define PWM_CLOCK 40000000
#define PWM_1000HZ 40000

#define MIN_PID 40
#define MAX_PID 39960
#define K_PID 32/64

volatile uint32_t desiredSpeed, measuredSpeed; // speed in units of 0.1 rps
volatile int32_t error;
volatile uint32_t count;
// Linear interpolation at low RPMs
uint32_t lowRPMDuties[] = {0, 0, 4800, 5250, 5400, 5650, 5800, 5950, 6100, 6250};


void Motor_Init()
{
    PWM0B_Init(PWM_1000HZ, 40);  // initialize PWM0, 1000 Hz, 0% duty
    Timer2_Init(800000);
    desiredSpeed = 10;
    measuredSpeed = 0;
}

void Motor_IncrementSpeed()
{
    // 100% duty @ 12V yeilds no more than 90 RPM.
    // Wrap around if desired speed exceeds physical limit.
    desiredSpeed = (desiredSpeed + 20) % 900;
}

void Motor_DecrementSpeed()
{
    if (desiredSpeed >= 20){
        desiredSpeed -= 20;
    } else {
        desiredSpeed = 0;
    }
}

uint32_t Motor_GetActual()
{
    return measuredSpeed;
}

uint32_t Motor_GetDesired()
{
    return desiredSpeed;
}

void Motor_Off()
{
    PWM0B_Duty(0);
}

uint32_t Motor_GetError(void)
{
    return error;
}

void Motor_ResetCount()
{
    count = 0;
}

void Timer2A_Handler(void)
{
    // pid is the integral of error
    static int32_t pid = 0;
    TIMER2_ICR_R =0x01;

    // Measure low speeds as 0 when the tach is not triggered.
    count++;
    if(count >= 3){
        measuredSpeed = 0;
    }
    else {
        measuredSpeed = Tach_GetSpeed();
    }

    error = desiredSpeed - measuredSpeed;
    // integrate error*K_PID
    pid += error * K_PID;

    // Cap off the integral (because data repr)
    if (pid < MIN_PID) {
        pid = MIN_PID;
    } else if (pid > MAX_PID) {
        pid = MAX_PID;
    }

    // Only use linear interpolation for edge cases,
    // otherwise, use PID control.
    if (desiredSpeed <= 90) {
        // TODO: improve hardcoded values (70, 80, 90)
        if (desiredSpeed % 10 == 0) {
            // use hardcoded value for some RPMs
            pid = lowRPMDuties[desiredSpeed/10];
        } else {
            // Linear interpolation for low RPMs
            uint8_t index = desiredSpeed / 10;
            uint32_t range = lowRPMDuties[index + 1] - lowRPMDuties[index];
            float proportion = (desiredSpeed % 10) / 10;
            pid = range * proportion + lowRPMDuties[index];
        }

        // setting the integral manually allows a smooth transition.
        // it's sort of a hack, but it works.
    }

    PWM0B_Duty(pid);
}
