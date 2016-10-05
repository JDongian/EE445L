// Music.h
#ifndef _MUSIC
#define _MUSIC
#include <stdint.h>
#include "Song.h"

#define Off     0		
#define C0	16.35  
#define D0	18.35  
#define E0	20.60  
#define F0	21.83  
#define G0	24.50  
#define Af0 25.96
#define A0	27.50 
#define Bf0 29.14
#define B0	30.87  
#define C1	32.70  
#define D1	36.71  
#define Ef1	38.89
#define E1	41.20  
#define F1	43.65  
#define G1	49.00  
#define Af1	51.91
#define A1	55.00  
#define Bf1	58.27
#define B1	61.74  
#define C2	65.41  
#define D2	73.42  
#define Ef2	77.78
#define E2	82.41  
#define F2	87.31  
#define G2	98.00  
#define Af2	103.83
#define A2	110.00 
#define Bf2	116.54
#define B2	123.47 
#define C3	130.81 
#define D3	146.83 
#define Ef3	155.56
#define E3	164.81 
#define F3	174.61 
#define G3	196.00 
#define Af3	207.65
#define A3	220.00 
#define Bf3	155.56
#define B3	233.08
#define C4	261.63 
#define D4	293.66 
#define Ef4	311.13
#define E4	329.63 
#define F4	349.23 
#define G4	392.00 
#define Af4	415.30
#define A4	440.00 
#define Bf4	466.16
#define B4	493.88 
#define C5	523.25 
#define D5	587.33 
#define Ef5    622.25
#define E5	659.25 
#define F5	698.46 
#define G5	783.99 
#define Af5    830.61
#define A5	880.00 
#define Bf5    932.33
#define B5	987.77 
#define C6	1046.50
#define D6	1174.66
#define E6	1318.51
#define F6	1396.91
#define G6	1567.98
#define A6	1760.00
#define B6	1975.53
#define C7	2093.00
#define D7	2349.32
#define E7	2637.02
#define F7	2793.83
#define G7	3135.96
#define A7	3520.00
#define B7	3951.07
#define C8	4186.01
#define D8	4698.63
#define E8	5274.04
#define F8	5587.65
#define G8	6271.93
#define A8	7040.00
#define B8	7902.13

//Times
#define semibreve		16*semiquaver
#define minim				8*semiquaver
#define crotchet		4*semiquaver
#define quaver			2*semiquaver
#define semiquaver	1

#define whole semibreve		
#define half minim				
#define quarter crotchet	
#define eighth quaver			
#define sixteenth semiquaver	

#define TEMPO 60.
#define TIME_UNIT 1000./((TEMPO/60)*4)


typedef struct MusicIndex {
  uint8_t noteIndex;
  double time;
} MusicIndex;

void PlayMusic(Song* s1, Song* s2);

void ChangeInstrument(void);

void PlayMusic2(Song* s);

#endif
