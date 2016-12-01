#ifndef __SENSE__
#define __SENSE__

typedef struct {
    int accel_x;
    int accel_y;
    int accel_z;
    int gyro_x;
    int gyro_y;
    int gyro_z;
} RobotState;

void update_state(RobotState* r);

#endif /* __SENSE__ */
