#ifndef _SOUND_
#define _SOUND_

#include <math.h>

#define INSTR_RES 32
#define INSTR_CENTER 1024
typedef uint16_t* Instrument;
uint16_t flute[INSTR_RES] = {
    1007, 1252, 1374, 1548, 1698, 1797, 1825, 1797,
    1675, 1562, 1383, 1219, 1092, 1007, 913, 890,
    833, 847, 810, 777, 744, 674, 598, 551,
    509, 476, 495, 533, 589, 659, 758, 876};

#endif /* _SOUND_ */
