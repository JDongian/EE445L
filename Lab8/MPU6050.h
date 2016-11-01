/* MPU6050.h
 * Trevor Eggenberger - tae498
 * Joshua Dong - jid295
 */

#ifndef __MPU6050__
#define __MPU6050__


void MPU6050_Init(void);
int16_t MPU6050_ReadAccel(char axis);
int16_t MPU6050_ReadGyro(char axis);

#endif /* __MPU6050__ */

