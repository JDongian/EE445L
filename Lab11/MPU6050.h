/* MPU6050.h
 * Trevor Eggenberger - tae498
 * Joshua Dong - jid295
 */

#ifndef __MPU6050__
#define __MPU6050__


void MPU6050_Init(void);
int16_t MPU6050_ReadAccel(char axis);
int16_t MPU6050_ReadGyro(char axis);
void MPU6050_SetGyroOffestX(uint16_t offset);
void MPU6050_SetGyroOffestY(uint16_t offset);
void MPU6050_SetGyroOffestZ(uint16_t offset);

#endif /* __MPU6050__ */

