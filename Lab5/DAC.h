// DAC.h
// Runs on LM4F120/TM4C123
// Low level drivers for the DAC

#ifndef _DAC
#define _DAC

//********DAC_Init*****************
// Initialization for DAC hardware.
// Input: calibration value (measured or theoretical value)
// Output: none
void DAC_Init(uint32_t base);

//********DAC_OutValue*****************
// inputs: desired output voltage
// outputs: output voltage (to hardware)
void DAC_OutValue(uint32_t value);

#endif /* _DAC */
