#ifndef _SOUND_
#define _SOUND_

#include <math.h>
#include <stdint.h>


#define INSTR_RES 32
#define INSTR_CENTER 1024
typedef uint16_t* Instrument;

uint16_t Wave_Value(Instrument inst, double index, uint16_t freq, uint8_t amp);

#endif /* _SOUND_ */
