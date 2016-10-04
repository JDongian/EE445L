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


Note guile_melody[] = {
  {Ef3, eighth}, {Ef3, sixteenth}, {D3, eighth}, 
  {D3, sixteenth}, {Ef3, eighth+quarter}+eighth, {Ef3, sixteenth}, {D3, sixteenth},
  
  {Ef3, eighth}, {Ef3, sixteenth}, {D3, eighth}, 
  {D3, sixteenth}, {Ef3, eighth+quarter}+eighth, {Ef3, sixteenth}, {D3, sixteenth},
  
  {Ef3, sixteenth}, {D3, eighth}, {Ef3, eighth},
  {D3, eighth}, {F3, eighth}, {F3, sixteenth},
  {Ef3, eighth}, {D3, eighth}, {B3, eighth},
  
  {Ef3, eighth}, {Ef3, sixteenth}, {D3, eighth}, 
  {D3, sixteenth}, {Ef3, eighth+quarter}+eighth, {Ef3, sixteenth}, {D3, sixteenth},
  
  {Ef3, eighth}, {Ef3, sixteenth}, {D3, eighth}, 
  {D3, sixteenth}, {Ef3, eighth+quarter}+eighth, {Ef3, sixteenth}, {D3, sixteenth},
  
  {Ef3, sixteenth}, {D3, eighth}, {Ef3, eighth},
  {D3, eighth}, {F3, eighth}, {F3, sixteenth},
  {Ef3, eighth}, {D3, eighth}, {B3, eighth},
  
  {C4, quarter+eighth+quarter}, {D4, eighth}, {Ef4, sixteenth}, {F4, eighth+sixteenth},
  
  {G4, eighth+sixteenth}, {F4, sixteenth}, {Off, eighth},
  {Bf3, quarter}, {Af3, eighth}, {G4, sixteenth}, {Af3, eighth+sixteenth},
  
  {D4, quarter}, {Ef4, eighth+sixteenth}, {F4, sixteenth},
  {Off, eighth}, {Bf3, eighth}, {D4, eighth}, {F4, eighth},
  
  {A5f, quarter}, {B5f, eighth+sixteenth}, {G4, sixteenth},
  {Off, eighth}, {G4, eighth}, {F4, sixteenth}, {D4, eighth+sixteenth},
  
  {C4, quarter+eighth+quarter}, {D4, eighth}, {Ef4, sixteenth}, {F4, eighth+sixteenth},
  
  {G4, eighth+ sixteenth}, {A4, sixteenth},{Off, eighth}, {F4, quarter}, 
  {F4, eighth}, {G4, sixteenth}, {A4, eighth+sixteenth},
  
  {B5f, quarter+eighth+quarter}, {C5, eighth}, {D5, sixteenth}, 
  {E5F, eithth+sixteenth}, 
  
  {G5, quarter}, {F5, quarter}, {D5, quarter}, {B5f, quarter},

  {C5, quarter+eighth+quarter}, {D5, eighth}, {E5f, sixteenth}, 
  {F5, eighth+sixteenth},
  
  {C5, quarter+eighth+quarter}, {D5, eighth}, {E5f, sixteenth}, 
  {F5, eighth+sixteenth},
  
  {G5, whole+whole}, 
  
  {A5, quarter+eighth+quarter}, {G5, eighth}, {F5, sixteenth},
  {G5, eighth+sixteenth},
  
  {G5, quarter}, {F5, quarter}, {D5, quarter}, {B5f, quarter},
  
  {C4, quarter}, {G4, whole+half}, {B5f, eighth}, {B5f, eighth},  

};

Note GuilesThemeTuba[] = {
  {C2, half+eighth}, {F2, sixteenth}, {Ef2, sixteenth},
  {F2, sixteenth}, {G2, sixteenth}, {F2, sixteenth}, {D2, sixteenth},
  
  {C2, half+eighth}, {F2, sixteenth}, {Ef2, sixteenth},
  {F2, sixteenth}, {G2, sixteenth}, {F2, sixteenth}, {D2, sixteenth},
  
  {F1, sixteenth}, {F1, eighth}, {F1, eighth},{F1, eighth},
  {G1, eighth}, {G1, sixteenth}, {G1, eighth},{G1, eighth},
  {Bf1, eighth},
  
  {C2, eighth}, {C3, sixteenth}, {C3, eighth}, 
  {C2, eighth}, {C3, eighth},{C2, sixteenth}, 
  {F2, sixteenth}, {Ef2, sixteenth}, {F2, sixteenth}, 
  {G2, sixteenth}, {F2, sixteenth}, {D2, sixteenth},
  
  {Af1, eighth}, {Af2, sixteenth}, {Af2, eighth}, 
  {Af1, eighth}, {Af2, eighth}, {Af1, sixteenth}, 
  {Ef2, sixteenth}, {G2, sixteenth}, {Af2, sixteenth},
  {Af2, eighth}, {Af2, sixteenth},
  
  {F1, sixteenth}, {F1, eighth}, {F1, eighth},{F1, eighth},
  {G1, eighth}, {G1, sixteenth}, {G1, eighth},{G1, eighth},
  {Bf1, eighth},
  
  {C2, sixteenth}, {C2, eighth},  {C2, sixteenth}, {C2, eighth},
  {C2, sixteenth}, {C2, eighth}, {Off, sixteenth}, 
  {C2, eighth}, {C2, sixteenth}, {C2, eighth}, {C2, sixteenth},
  
  {Af1, sixteenth}, {Af1, eighth},  {Af1, sixteenth}, {Af1, eighth},
  {Af1, sixteenth}, {Af1, eighth}, {Off, sixteenth}, 
  {Af1, eighth}, {Af1, sixteenth}, {Af1, eighth}, {Af1, sixteenth},
  
  {Bf1, sixteenth}, {Bf1, eighth},  {Bf1, sixteenth}, {Bf1, eighth},
  {Bf1, sixteenth}, {Bf1, eighth}, {Off, sixteenth}, 
  {Bf1, eighth}, {Bf1, sixteenth}, {Bf1, eighth}, {Bf1, sixteenth},
  
  {G1, sixteenth}, {G1, eighth},  {G1, sixteenth}, {G1, eighth},
  {G1, sixteenth}, {G1, eighth}, {Off, sixteenth}, 
  {C2, sixteenth}, {D2, sixteenth}, {Ef2, sixteenth}, 
  {D2, sixteenth}, {Bf2, sixteenth}, {C2, sixteenth}, 
  
  {C2, sixteenth}, {C2, eighth},  {C2, sixteenth}, {C2, eighth},
  {C2, sixteenth}, {C2, eighth}, {Off, sixteenth}, 
  {C2, eighth}, {C2, sixteenth}, {C2, eighth}, {C2, sixteenth},
  
  {Af1, sixteenth}, {Af1, eighth},  {Af1, sixteenth}, {Af1, eighth},
  {Af1, sixteenth}, {Af1, eighth}, {Off, sixteenth}, 
  {Af1, eighth}, {Af1, sixteenth}, {Af1, eighth}, {Af1, sixteenth},
};


uint8_t volumes_guile_melody[90] = {1};
guile_melody.notes = notes_guile_melody;
guile_melody.volumes = volumes_guile_melody;
guile_melody.tempo = 80;
guile_melody.instr = &flute;

