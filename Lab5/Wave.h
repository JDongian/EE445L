#ifndef _SOUND_
#define _SOUND_

#include <stdint.h>
double scale(uint16_t v, double s);

uint16_t envelope(uint16_t original, double proportion);
	
#endif /* _SOUND_ */
