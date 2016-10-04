#ifndef _SOUND_
#define _SOUND_

#include <math.h>
#include <stdint.h>


#define INSTR_RES 32
#define INSTR_CENTER 1024
#define WAVE_RES 200
typedef const uint16_t* Instrument;

uint16_t Wave_Value(Instrument inst, double index, uint16_t freq, uint8_t amp);

double scale(uint16_t v, double s);
	
#endif /* _SOUND_ */
