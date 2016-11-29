#ifndef __MOTOR__
#define __MOTOR__

#define RESOLUTION 1000

typedef float Speed;
typedef enum Directions {NONE, FORWARD, BACKWARD} Dir;
typedef enum Motor {PORT, STARBOARD} Side;

typedef struct {
    Dir direction;
    int speed;
} MotorState;


void motor_set(Side, Dir, int);
void motor_run();
void motor_init(void);
void motor_off(void);

#endif /* __MOTOR__ */
