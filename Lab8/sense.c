#include <stdint.h> 
#include "sense.h"
#include "MPU6050.h"

void update_state(RobotState* r){
	r->gyro_x = MPU6050_ReadGyro('x');
  	r->gyro_y = MPU6050_ReadGyro('y');
  	r->gyro_z = MPU6050_ReadGyro('z');
	r->accel_x = MPU6050_ReadAccel('x');
	r->accel_y = MPU6050_ReadAccel('y');
	r->accel_z = MPU6050_ReadAccel('z');
}
