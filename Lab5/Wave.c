#include "Wave.h"
#include "systick.h"

// Linear interpolation
uint16_t interpolate(Instrument inst,
                     uint16_t freq, double index, uint8_t res) {
    double fpart, ipart;
    fpart = modf(fmod(freq * index * (res + 1) , (res + 1)), &ipart);
    uint16_t begin = inst[(int)ipart];
    // modulo again in case of edge case
    uint16_t end = inst[(int)fmod(ipart + 1, res + 1)];
    return begin + (end - begin) * fpart;
}

void Wave_Init() {
    // Could possibly extrapolate values for instruments here.
    return;
}

// Return value for the DAC to use at a given time
// inst/DAC[32]
// index/sec
// freq/Hz
// amp
uint16_t Wave_Value(Instrument inst, double index, uint16_t freq, uint8_t amp) {
    if (freq == 0) {
        return 0;
    } else {
        return amp * interpolate(inst, freq, index, INSTR_RES);
    }
}
