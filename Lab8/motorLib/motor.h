#ifndef __MOTOR__
#define __MOTOR__

typedef float Speed;
typedef enum Directions {NONE, FORWARD, BACKWARD} Dir;
typedef enum Motor {PORT, STARBOARD} Side;

typedef struct {
    Dir direction;
    Speed speed;
} MotorState;


void motor_set(Side, Dir);
void motor_run(void);
void motor_init(void);

#endif /* __MOTOR__ */
