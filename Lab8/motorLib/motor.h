#ifndef __MOTOR__
#define __MOTOR__

typedef enum Directions {NONE, FORWARD, BACKWARD} Dir;
typedef enum Motor {PORT, STARBOARD} Side;

struct MotorState {
    Dir left_state; // port side
    Dir right_state; // starboard side
};


void motor_set(Side, Dir);
void motor_run(void);

#endif /* __MOTOR__ */
