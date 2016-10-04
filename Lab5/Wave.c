#include "Wave.h"
#include "Wave.h"
#include "systick.h"

double scale(uint16_t v, double s) {
  // Appropriate scaling for non-zero-centered values
  return (v - INSTR_CENTER) * s + INSTR_CENTER;
}

uint16_t envelope(uint16_t original, double proportion){
  // Simple hard attack, trailing decay envelope
  uint16_t s = scale(original, (1-proportion));
  return s;
}

//uint16_t add_harmonic(uint16_t h1, uint16_t h2, uint16_t h3) {
//  return (uint16_t)(scale(h1, 0.9) + scale(h2, 0.1));// + scale(h3, 0.1));
//}
