#include <stdint.h>
#include "PWM.h"

#define PWM_CLOCK 40000000
#define PWM_1000HZ 40000

void Motor_Init(){
	 PWM0B_Init(PWM_1000HZ, 0);  // initialize PWM0, 1000 Hz, 0% duty
}
	
void Motor_SetSpeed(uint16_t duty){
	PWM0B_Duty(duty);
}

void Motor_Off(){
	PWM0B_Duty(0);
}
