#include "Music.h"
#include "Wave.h"
#include "MAX5353.h"
#include "../inc/tm4c123gh6pm.h"

double sysTime = 0;
MusicIndex i = {0, 0};
int j = 0;

#define PF1       (*((volatile uint32_t *)0x40025008))

const uint16_t flute[INSTR_RES] = {
    1007, 1252, 1374, 1548, 1698, 1797, 1825, 1797,
    1675, 1562, 1383, 1219, 1092, 1007, 913, 890,
    833, 847, 810, 777, 744, 674, 598, 551,
    509, 476, 495, 533, 589, 659, 758, 876};

const uint16_t Oboe[64]={
1024, 1024, 1014, 1008, 1022, 1065, 1093, 1006, 858, 711, 612, 596, 672, 806, 952, 1074, 1154, 1191, 
1202, 1216, 1236, 1255, 1272, 1302, 1318, 1299, 1238, 1140, 1022, 910, 827, 779, 758, 757, 782, 856, 
972, 1088, 1177, 1226, 1232, 1203, 1157, 1110, 1067, 1028, 993, 958, 929, 905, 892, 900, 940, 1022, 
1125, 1157, 1087, 965, 836, 783, 816, 895, 971, 1017};

const uint16_t Trumpet[64]={
  987, 1049, 1090, 1110, 1134, 1160, 1139, 1092, 1070, 1042, 1035, 1029, 1008, 1066, 1150, 1170, 1087, 915, 679, 372, 151, 
  558, 1014, 1245, 1260, 1145, 1063, 984, 934, 960, 1027, 1077, 1081, 1074, 1064, 1042, 1010, 974, 968, 974, 994, 1039, 
  1094, 1129, 1125, 1092, 1056, 1056, 1082, 1059, 1046, 1058, 1061, 1045, 1034, 1050, 1094, 1112, 1092, 1063, 1053, 1065, 1052, 992};

	
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

Note GuilesTheme2[] = {
    {Ds5, quaver}, {Ds5, semiquaver}, {D5, semiquaver}, {Off, semiquaver},
        {D5, semiquaver}, {Ds5, quaver+crotchet}, {D5, quaver},
    {Ds5, quaver}, {Ds5, semiquaver}, {D5, semiquaver}, {Off, semiquaver},
        {D5, semiquaver}, {Ds5, quaver+crotchet}, {D5, quaver},
    {Ds5, semiquaver}, {D5, quaver}, {Ds5, semiquaver}, {Off, semiquaver},
        {D5, quaver}, {F5, semiquaver}, {Off, semiquaver}, {F5, semiquaver},
        {Ds5, quaver}, {D5, quaver}, {As4, quaver},
    {Ds5, quaver}, {Ds5, semiquaver}, {D5, semiquaver}, {Off, semiquaver},
        {D5, semiquaver}, {Ds5, quaver+crotchet}, {D5, quaver},
    {Ds5, quaver}, {Ds5, semiquaver}, {D5, semiquaver}, {Off, semiquaver},
        {D5, semiquaver}, {Ds5, quaver+crotchet}, {D5, quaver},
    {Ds5, semiquaver}, {D5, quaver}, {Ds5, semiquaver}, {Off, semiquaver},
        {D5, quaver}, {F5, semiquaver}, {Off, semiquaver}, {F5, semiquaver},
        {Ds5, quaver}, {D5, quaver}, {As4, quaver},
    {C4, minim+quaver}, {D4, quaver}, {Ds4, semiquaver},
        {F4, quaver+semiquaver},
    {G4, quaver+semiquaver}, {G4, semiquaver}, {F4, quaver},
        {As4, quaver+quaver}, {Gs4, quaver}, {G4, semiquaver},
        {Gs4, quaver+semiquaver},
    {D4, quaver+semiquaver}, {Ds4, semiquaver+quaver}, {F4, quaver},
        {Off, quaver}, {As2, quaver}, {D4, quaver}, {F4, quaver},
    {Gs4, quaver+semiquaver}, {As4, semiquaver+quaver}, {G4, quaver},
        {Off, quaver}, {G4, quaver}, {F4, quaver}, {D4, quaver},
    {C4, semibreve},
    {A5, 0}
};

Note Lamb[] = {
    {B4, quaver}, {A4, quaver},{G4, quaver},{A4, quaver},
    {B4, quaver},{B4, quaver}, {B4, crotchet}, {A4,quaver},
    {A4, quaver},{A4, crotchet}, {B4, quaver}, {D5, quaver}, {D5, crotchet}, {B4,quaver},
		{A4, quaver}, {G4, quaver}, {A4, crotchet}, {B4,quaver},{B4,quaver},
		{B4,quaver}, {B4,quaver},{A4, quaver}, {Off, semiquaver},{A4, quaver}, 
		{B4, quaver}, {A4, quaver}, {G4, minim}
};


const uint16_t wave2[32] = {
  2048*2,2448*2,2832*2,3186*2,3496*2,3751*2,3940*2,4057*2,4095*2,4057*2,3940*2,
  3751*2,3496*2,3186*2,2832*2,2448*2,2048*2,1648*2,1264*2,910*2,600*2,345*2,
  156*2,39*2,0*2,39*2,156*2,345*2,600*2,910*2,1264*2,1648*2};

double tempo(int duration){
	return duration/8.;
}


double scale(uint16_t v, double s) {
	return (v - INSTR_CENTER) * s + INSTR_CENTER;
}

uint16_t h(uint16_t h1, uint16_t h2, uint16_t h3) {
	return (uint16_t)(scale(h1, 0.9) + scale(h2, 0.1));// + scale(h3, 0.1));
}

void Timer0A_Handler(void) {
	TIMER0_ICR_R = TIMER_ICR_TATOCINT;
	uint16_t val = 0;
	uint16_t val2 = 0;
	if (fmod(sysTime,1000) == 0) {
		PF1 ^= 0x02;
	}
	
	double currentTime = sysTime / 1000;
  double delta = currentTime - i.time;

	if (delta >= tempo(GreenHills[i.note].time)) {
      i.note = (i.note + 1) % 59;
      i.time = currentTime;
      delta = 0;
	}
	//val2 = Wave_Value(Oboe, delta, GreenHills2[i.note].frequency, 1);
	//uint16_t harmonic = h(Wave_Value(Oboe, delta, GreenHills[i.note].frequency, 1),
	             //Wave_Value(Oboe, delta, GreenHills[i.note].frequency * 2, 1),
	            // Wave_Value(Oboe, delta, GreenHills[i.note].frequency / 2., 1));
	val = Wave_Value(flute, delta, GreenHills[i.note].frequency, 1);
  DAC_OutValue(val);
	sysTime += 1./WAVE_RES;
}

void Timer1A_Handler() {
	TIMER1_ICR_R = TIMER_ICR_TATOCINT;
	DAC_OutValue(flute[j]);
	j = (j+1) % 64;
}

void Timer2A_Handler() {
	TIMER2_ICR_R = TIMER_ICR_TATOCINT;
	TIMER1_CTL_R = 0x00000000;    // 10) disable TIMER1A
	TIMER1_TAILR_R = 80000000/GreenHills[i.note].frequency/64;
	TIMER1_CTL_R = 0x00000001;    // 10) enable TIMER1A
	i.note = (i.note+1) % 59;
	uint16_t time = GreenHills[i.note].time ;
	uint32_t reload = time*150*80000;
	TIMER2_TAILR_R = reload;
}

void Play_Music(){

}
