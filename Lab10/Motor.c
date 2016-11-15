#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "tach.h"
#include "PWM.h"
#include "Timers.h"

#define PWM_CLOCK 40000000
#define PWM_1000HZ 40000

#define MIN_ 40
#define MAX_ 39960

volatile uint32_t desiredSpeed, measuredSpeed;

volatile int32_t error;

volatile uint32_t count;

void Motor_Init(){
	PWM0B_Init(PWM_1000HZ, 40);  // initialize PWM0, 1000 Hz, 0% duty
	Timer2_Init(800000);
	desiredSpeed = 10;
	measuredSpeed = 0;
}
	
void Motor_IncrementSpeed(){
	desiredSpeed = (desiredSpeed + 20) % 900;
}

void Motor_DecrementSpeed(){
	if (desiredSpeed - 20 > 0){
		desiredSpeed = (desiredSpeed - 20);
	}
	else{
		desiredSpeed = 0;
	}
}

uint32_t Motor_GetActual(){
	return measuredSpeed;
}

uint32_t Motor_GetDesired(){
	return desiredSpeed;
}

void Motor_Off(){
	PWM0B_Duty(0);
}

uint32_t Motor_GetError(void){
	return error;
}

void Motor_ResetCount(){
	count = 0;
}

uint32_t lowRPMDuties[] = {0, 0, 4800, 5250, 5400, 5650, 5800, 5950, 6100, 6250};

void Timer2A_Handler(void){
	static int32_t pid = 0;
	TIMER2_ICR_R =0x01;
	
	count++;
	if(count >= 3){
		measuredSpeed = 0;
	}
	else {
		measuredSpeed = Tach_GetSpeed();
	}
	if (desiredSpeed <= 90){
		//PWM0B_Duty(desiredSpeed*4800);
		if (desiredSpeed % 10 == 0) PWM0B_Duty(lowRPMDuties[desiredSpeed/10]);
		else {
			uint8_t index = desiredSpeed/10;
			uint32_t range = lowRPMDuties[index+1] - lowRPMDuties[index];
			float proportion = (desiredSpeed % 10) / 10;
			PWM0B_Duty(range * proportion + lowRPMDuties[index]);
		}
	}
	else {
		PWM0B_Duty(pid);
	}
}
