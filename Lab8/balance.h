#ifndef __BALANCE__
#define __BALANCE__

#include "sense.h"


typedef struct {
    double left_adjust; // port side
    double right_adjust; // starboard side
} Vector;


// calculate adjustment needed to rebalance
// speed determines righting speed
Vector calc_rebalance(RobotState state, double speed);

// calculate adjustment needed to go forward
Vector calc_forward(RobotState state, double speed);

// calculate adjustment needed to go backward
Vector calc_forward(RobotState state, double speed);

// calculate adjustment needed to rotate (degrees)
Vector calc_forward(RobotState state, double angle);

#endif /* __BALANCE__ */
