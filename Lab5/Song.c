#include "Song.h"
#include "Instrument.h"


typedef struct Song {
  Note notes[1024];
  uint8_t volumes[1024];
  uint16_t tempo;
  Instrument* instr;
} Song;


//// GREEN HILLS
Note notes_greenhills[] = {
  {Off, 0},
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
  {Off, 0}
};
uint8_t volumes_greenhills[] = {
  0,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,
  1,1,1,0
};
greenhills.notes = notes_greenhills;
greenhills.volumes = volumes_greenhills;
greenhills.tempo = 120;
greenhills.instr = &trumpet;
////
//// TEST REPEAT 
Note notes_test_repeat[] = {
  {Off, 0},
  {B4, quaver}, {B4, quaver}, {B4, quaver}, {B4, quaver}
};
uint8_t volumes_test_repeat[] = {
  0,1,1,1,1
};
test_repeat.notes = notes_test_repeat;
test_repeat.volumes = volumes_test_repeat;
test_repeat.tempo = 60;
test_repeat.instr = &flute;
////

/*
song_scale[] = {
  {Off, 0},
  {C4, quaver}, {D4, quaver}, {E4, quaver}, {F4, quaver}, 
  {G4, quaver},{A5, quaver},{B5, quaver}, {C5, quaver+quaver},
  {B5, quaver}, {A5, quaver}, {G4, quaver}, {F4, quaver}, 
  {E4, quaver}, {D5, quaver}, {C4, quaver},
};
*/

