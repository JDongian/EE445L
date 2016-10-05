#include "Instrument.h"

double scale(uint16_t v, double s) {
	return (v - INSTR_CENTER) * s + INSTR_CENTER;
}

uint16_t envelope(uint16_t original, double proportion){
	uint16_t s = scale(original, (1-proportion));
	return s;
}
