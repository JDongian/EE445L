#include "Music.h"
#include "Wave.h"
#include "MAX5353.h"
#include "../inc/tm4c123gh6pm.h"

double currentTimeMili = 0;
MusicIndex i = {0, 0};

#define PF1       (*((volatile uint32_t *)0x40025008))

uint16_t flute[INSTR_RES] = {
    1007, 1252, 1374, 1548, 1698, 1797, 1825, 1797,
    1675, 1562, 1383, 1219, 1092, 1007, 913, 890,
    833, 847, 810, 777, 744, 674, 598, 551,
    509, 476, 495, 533, 589, 659, 758, 876};

Note GreenHills[] = {
    {C5, quaver}, {A4, crotchet}, {C5, quaver}, 
    {B4, crotchet}, {C5, quaver}, {B4, crotchet}, {G4, minim+quaver}, 
    {A4, quaver}, {E5, quaver}, {D5, crotchet}, {C5, quaver},
    {B4, crotchet}, {C5, quaver}, {B4, crotchet}, {G4, minim+crotchet+quaver},

    {C5, quaver}, {A4, crotchet}, {C5, quaver}, 
    {B4, crotchet}, {C5, quaver}, {B4, crotchet}, {G4, minim+quaver}, 		
    {A4, crotchet}, {F4, crotchet}, {A4, quaver},
    {G4, crotchet}, {A4, quaver}, {G4, crotchet},	{C4, minim+crotchet},
    {C5, quaver}, {A4, crotchet}, {C5, quaver}, 
    {B4, crotchet}, {C5, quaver}, {B4, crotchet}, {G4, minim+quaver}, 
    {A4, quaver}, {E5, quaver}, {D5, crotchet}, {C5, quaver},
    {B4, crotchet}, {C5, quaver}, {B4, crotchet}, {G4, minim+crotchet+quaver},

    {C5, quaver}, {A4, crotchet}, {C5, quaver}, 
    {B4, crotchet}, {C5, quaver}, {B4, crotchet}, {G4, minim+quaver}, 		
    {A4, crotchet}, {F4, crotchet}, {A4, quaver},
    {G4, crotchet}, {A4, quaver}, {G4, crotchet},	{C4, minim+crotchet},
    {A4, 0}
};
Note GuilesTheme[] = {
    {Ds4, quaver}, {Ds4, semiquaver}, {D4, semiquaver}, {Off, semiquaver},
        {D4, semiquaver}, {Ds4, quaver+crotchet}, {D4, quaver},
    {Ds4, quaver}, {Ds4, semiquaver}, {D4, semiquaver}, {Off, semiquaver},
        {D4, semiquaver}, {Ds4, quaver+crotchet}, {D4, quaver},
    {Ds4, semiquaver}, {D4, quaver}, {Ds4, semiquaver}, {Off, semiquaver},
        {D4, quaver}, {F4, semiquaver}, {Off, semiquaver}, {F4, semiquaver},
        {Ds4, quaver}, {D4, quaver}, {As3, quaver},
    {Ds4, quaver}, {Ds4, semiquaver}, {D4, semiquaver}, {Off, semiquaver},
        {D4, semiquaver}, {Ds4, quaver+crotchet}, {D4, quaver},
    {Ds4, quaver}, {Ds4, semiquaver}, {D4, semiquaver}, {Off, semiquaver},
        {D4, semiquaver}, {Ds4, quaver+crotchet}, {D4, quaver},
    {Ds4, semiquaver}, {D4, quaver}, {Ds4, semiquaver}, {Off, semiquaver},
        {D4, quaver}, {F4, semiquaver}, {Off, semiquaver}, {F4, semiquaver},
        {Ds4, quaver}, {D4, quaver}, {As3, quaver},
    {C3, minim+quaver}, {D3, quaver}, {Ds3, semiquaver},
        {F3, quaver+semiquaver},
    {G3, quaver+semiquaver}, {G3, semiquaver}, {F3, quaver},
        {As3, quaver+quaver}, {Gs3, quaver}, {G3, semiquaver},
        {Gs3, quaver+semiquaver},
    {D3, quaver+semiquaver}, {Ds3, semiquaver+quaver}, {F3, quaver},
        {Off, quaver}, {As2, quaver}, {D3, quaver}, {F3, quaver},
    {Gs3, quaver+semiquaver}, {As3, semiquaver+quaver}, {G3, quaver},
        {Off, quaver}, {G3, quaver}, {F3, quaver}, {D3, quaver},
    {C3, semibreve},
    {A4, 0}
};

const uint16_t wave2[32] = {
  2048*2,2448*2,2832*2,3186*2,3496*2,3751*2,3940*2,4057*2,4095*2,4057*2,3940*2,
  3751*2,3496*2,3186*2,2832*2,2448*2,2048*2,1648*2,1264*2,910*2,600*2,345*2,
  156*2,39*2,0*2,39*2,156*2,345*2,600*2,910*2,1264*2,1648*2};

void Timer0A_Handler(void) {
	TIMER0_ICR_R = TIMER_ICR_TATOCINT;
	if (fmod(currentTimeMili,1000) == 0) PF1 ^= 0x02;
	/*
  static int i = 0;
	DAC_OutValue(wave2[i&0x1F]);
	i++;
	*/
	
	
	currentTimeMili += 1;
        double currentTime = currentTimeMili / 1000;
        double delta = currentTime - i.time;
        if (delta >= GreenHills[i.note].time) {
            i.note += 1;
            i.time = currentTime;
            delta = 0;
				}
				uint16_t val = Wave_Value(flute, delta, GreenHills[i.note].frequency, 1);
        DAC_OutValue(val);
	
	
}
