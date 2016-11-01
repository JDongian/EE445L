#include <stdint.h>
#include "MPU6050.h"
#include "i2c.h"

#define MPU6050_ADDRESS 0x68

#define MPU6050_ACCEL_XOUT_H 0x3B
#define MPU6050_ACCEL_XOUT_L 0x3C
#define MPU6050_ACCEL_YOUT_H 0x3D
#define MPU6050_ACCEL_YOUT_L 0x3E
#define MPU6050_ACCEL_ZOUT_H 0x3F
#define MPU6050_ACCEL_ZOUT_L 0x40

#define MPU6050_GYRO_XOUT_H 0x43
#define MPU6050_GYRO_XOUT_L 0x44
#define MPU6050_GYRO_YOUT_H 0x45
#define MPU6050_GYRO_YOUT_L 0x46
#define MPU6050_GYRO_ZOUT_H 0x37
#define MPU6050_GYRO_ZOUT_L 0x48

void MPU6050_Init(void){
    unsigned char confpacket[] = {0x00};
    i2c_init();
    i2c_sendPacket(MPU6050_ADDRESS, 0x6B, 1, confpacket);
}

int16_t MPU6050_ReadAccel(char axis){
    int16_t accel_reading = 0;
    unsigned char axis_data_h;
    unsigned char axis_data_l;
    unsigned char axis_address_h;

    if((axis >= 'x') && (axis <= 'z')){
        axis_address_h = MPU6050_ACCEL_XOUT_H + (axis - 'x')*2;
        axis_data_h = i2c_readData(MPU6050_ADDRESS,axis_address_h);
        axis_data_l = i2c_readData(MPU6050_ADDRESS,axis_address_h+1);
        accel_reading = (axis_data_h << 8) | (axis_data_l);
    }
    return accel_reading;
}

int16_t MPU6050_ReadGyro(char axis){
    int16_t gyro_reading = 0;
    unsigned char axis_data_h;
    unsigned char axis_data_l;
    unsigned char axis_address_h;

    if((axis >= 'x') && (axis <= 'z')){
        axis_address_h = MPU6050_GYRO_XOUT_H + (axis - 'x')*2;
        axis_data_h = i2c_readData(MPU6050_ADDRESS,axis_address_h);
        axis_data_l = i2c_readData(MPU6050_ADDRESS,axis_address_h+1);
        gyro_reading = (axis_data_h << 8) | (axis_data_l);
    }
    return gyro_reading;
}
