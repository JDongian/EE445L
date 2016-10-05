#include "Song.h"
#include "Instrument.h"
#include "Music.h"

Song guiles_melody;
Song greenhills;
Song test_scale;
Song guiles_tuba;
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

Note notes_scale[] = {
  {C4, quarter}, {D4, quarter}, {E4, quarter}, 
  {F4, quarter}, {G4, quarter}, {A4, quarter}, 
  {B4, quarter}, {C5, quarter},
  {B4, quarter}, {A4, quarter}, 
  {G4, quarter}, {F4, quarter}, {E4, quarter}, 
  {D4, quarter}, {C4, quarter}
};
uint8_t volumes_scale[] = {
  1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1
};


uint8_t volumes_guile_melody[90] = {1};
uint8_t volumes_guile_tuba[90] = {1};

Note notes_guiles_melody[] = {
  {Ef3, eighth}, {Ef3, sixteenth}, {D3, eighth}, 
  {D3, sixteenth}, {Ef3, eighth+quarter+eighth}, {Ef3, sixteenth}, {D3, sixteenth},
  
  {Ef3, eighth}, {Ef3, sixteenth}, {D3, eighth}, 
  {D3, sixteenth}, {Ef3, eighth+quarter+eighth}, {Ef3, sixteenth}, {D3, sixteenth},
  
  {Ef3, sixteenth}, {D3, eighth}, {Ef3, eighth},
  {D3, eighth}, {F3, eighth}, {F3, sixteenth},
  {Ef3, eighth}, {D3, eighth}, {Bf3, eighth},
  
  {Ef3, eighth}, {Ef3, sixteenth}, {D3, eighth}, 
  {D3, sixteenth}, {Ef3, eighth+quarter+eighth}, {Ef3, sixteenth}, {D3, sixteenth},
  
  {Ef3, eighth}, {Ef3, sixteenth}, {D3, eighth}, 
  {D3, sixteenth}, {Ef3, eighth+quarter+eighth}, {Ef3, sixteenth}, {D3, sixteenth},
  
  {Ef3, sixteenth}, {D3, eighth}, {Ef3, eighth},
  {D3, eighth}, {F3, eighth}, {F3, sixteenth},
  {Ef3, eighth}, {D3, eighth}, {Bf3, eighth},
  
  {C2, quarter+eighth+quarter}, {D2, eighth}, {Ef2, sixteenth}, {F2, eighth+sixteenth},
  
  {G2, eighth+sixteenth}, {F2, sixteenth}, {Off, eighth},
  {Bf2, quarter}, {Af2, eighth}, {G2, sixteenth}, {Af2, eighth+sixteenth},
  
  {D2, quarter}, {Ef2, eighth+sixteenth}, {F2, sixteenth},
  {Off, eighth}, {Bf2, eighth}, {D2, eighth}, {F2, eighth},
  
  {Af2, quarter}, {Bf2, eighth+sixteenth}, {G2, sixteenth},
  {Off, eighth}, {G2, eighth}, {F2, sixteenth}, {D2, eighth+sixteenth},
  
  {C2, quarter+eighth+quarter}, {D2, eighth}, {Ef2, sixteenth}, {F2, eighth+sixteenth},
  
  {G2, eighth+ sixteenth}, {A2, sixteenth},{Off, eighth}, {F2, quarter}, 
  {F2, eighth}, {G2, sixteenth}, {A2, eighth+sixteenth},
  
  {Bf2, quarter+eighth+quarter}, {C3, eighth}, {D3, sixteenth}, 
  {Ef3, eighth+sixteenth}, 
  
  {G3, quarter}, {F3, quarter}, {D3, quarter}, {Bf2, quarter},

  {C3, quarter+eighth+quarter}, {D3, eighth}, {Ef3, sixteenth}, 
  {F3, eighth+sixteenth},
  
  {C3, quarter+eighth+quarter}, {D3, eighth}, {Ef3, sixteenth}, 
  {F3, eighth+sixteenth},
  
  {G3, whole+whole}, 
  
  {Af3, quarter+eighth+quarter}, {G3, eighth}, {F3, sixteenth},
  {G3, eighth+sixteenth},
  
  {G3, quarter}, {F3, quarter}, {D3, quarter}, {Bf2, quarter},
  
  {C3, quarter}, {G2, whole+half}, {Bf2, eighth}, {Bf2, eighth},  

};

Note notes_guiles_tuba[] = {
  {C1, half+eighth}, {F1, sixteenth}, {Ef1, sixteenth},
  {F1, sixteenth}, {G1, sixteenth}, {F1, sixteenth}, {D1, sixteenth},
  
  {A0, half+eighth}, {Ef1, sixteenth}, {G1, sixteenth},
  {Af1, sixteenth}, {Af1, eighth}, {Af1, sixteenth}, 
  
  {F0, sixteenth}, {F0, eighth}, {F0, eighth},{F0, eighth},
  {G0, eighth}, {G0, sixteenth}, {G0, eighth},{G0, eighth},
  {Bf0, eighth},
  
  {C1, eighth}, {C2, sixteenth}, {C2, eighth}, 
  {C1, eighth}, {C2, eighth},{C1, sixteenth}, 
  {F1, sixteenth}, {Ef1, sixteenth}, {F1, sixteenth}, 
  {G1, sixteenth}, {F1, sixteenth}, {D1, sixteenth},
  
  {Af0, eighth}, {Af1, sixteenth}, {Af1, eighth}, 
  {Af0, eighth}, {Af1, eighth}, {Af0, sixteenth}, 
  {Ef1, sixteenth}, {G1, sixteenth}, {Af1, sixteenth},
  {Af1, eighth}, {Af1, sixteenth},
  
  {F0, sixteenth}, {F0, eighth}, {F0, eighth},{F0, eighth},
  {G0, eighth}, {G0, sixteenth}, {G0, eighth},{G0, eighth},
  {Bf0, eighth},
  
  {C1, sixteenth}, {C1, eighth},  {C1, sixteenth}, {C1, eighth},
  {C1, sixteenth}, {C1, eighth}, {Off, sixteenth}, 
  {C1, eighth}, {C1, sixteenth}, {C1, eighth}, {C1, sixteenth},
  
  {Af0, sixteenth}, {Af0, eighth},  {Af0, sixteenth}, {Af0, eighth},
  {Af0, sixteenth}, {Af0, eighth}, {Off, sixteenth}, 
  {Af0, eighth}, {Af0, sixteenth}, {Af0, eighth}, {Af0, sixteenth},
  
  {Bf0, sixteenth}, {Bf0, eighth},  {Bf0, sixteenth}, {Bf0, eighth},
  {Bf0, sixteenth}, {Bf0, eighth}, {Off, sixteenth}, 
  {Bf0, eighth}, {Bf0, sixteenth}, {Bf0, eighth}, {Bf0, sixteenth},
  
  {G0, sixteenth}, {G0, eighth},  {G0, sixteenth}, {G0, eighth},
  {G0, sixteenth}, {G0, eighth}, {Off, sixteenth}, 
  {C1, sixteenth}, {D1, sixteenth}, {Ef1, sixteenth}, 
  {D1, sixteenth}, {Bf0, sixteenth}, {C1, sixteenth}, 
  
  {C1, sixteenth}, {C1, eighth},  {C1, sixteenth}, {C1, eighth},
  {C1, sixteenth}, {C1, eighth}, {Off, sixteenth}, 
  {C1, eighth}, {C1, sixteenth}, {C1, eighth}, {C1, sixteenth},
  
  {Af0, sixteenth}, {Af0, eighth},  {Af0, sixteenth}, {Af0, eighth},
  {Af0, sixteenth}, {Af0, eighth}, {Off, sixteenth}, 
  {Af0, eighth}, {Af0, sixteenth}, {Af0, eighth}, {Af0, sixteenth},
	
	 {Bf0, sixteenth}, {Bf0, eighth},  {Bf0, sixteenth}, {Bf0, eighth},
  {Bf0, sixteenth}, {Bf0, eighth}, {Off, sixteenth}, 
  {Bf0, eighth}, {Bf0, sixteenth}, {Bf0, eighth}, {Bf0, sixteenth},
	
	{G0, quarter}, {G0, quarter+eighth}, {C1, sixteenth}, {D1, sixteenth}, 
	{G1, sixteenth}, {Ef1, sixteenth}, {C1, sixteenth}, {Ef1, sixteenth},
	
	{Af0, eighth+sixteenth}, {Af0, eighth}, {Af0, eighth}, {Af0, sixteenth}, 
	{Af0, eighth+sixteenth}, {Af0, eighth}, {Af0, eighth}, {Af0, sixteenth}, 
	
	{Bf0, eighth+sixteenth}, {Bf0, eighth}, {Bf0, eighth}, {Bf0, sixteenth}, 
	{Bf0, eighth+sixteenth}, {Bf0, eighth}, {Bf0, eighth}, {Bf0, sixteenth}, 
	
	{C1, sixteenth}, {C1, eighth}, {C1, half}, {C1, sixteenth}, 
	{C1, eighth+sixteenth}, {C1, sixteenth}, 
	
	{C1, eighth}, {C1, sixteenth}, {C1, eighth}, {C1, sixteenth}, 
	{C1, half+eighth},

	{Af0, eighth+sixteenth}, {Af0, eighth}, {Af0, eighth}, {Af0, sixteenth}, 
	{Af0, eighth+sixteenth}, {Af0, eighth}, {Af0, eighth}, {Af0, sixteenth}, 
	
	{Bf0, eighth+sixteenth}, {Bf0, eighth}, {Bf0, eighth}, {Bf0, sixteenth}, 
	{Bf0, eighth+sixteenth}, {Bf0, eighth}, {Bf0, eighth}, {Bf0, sixteenth}, 
	
	{C1, sixteenth}, {C1, eighth}, {C1, half}, {C1, sixteenth}, {C1, eighth+sixteenth},
	{C1, sixteenth}
};

/*
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
*/


void init_songs(){
	greenhills.notes = notes_greenhills;
	greenhills.volumes = volumes_greenhills;
	greenhills.tempo = 150;
	greenhills.instr = &oboe;
	greenhills.length = 60;
	greenhills.timeUnit = 80000*(60000/greenhills.tempo/4);
	
	guiles_melody.notes = notes_guiles_melody;
	guiles_melody.volumes = volumes_guile_melody;
	guiles_melody.tempo = 100;
	guiles_melody.instr = &flute;
	guiles_melody.length = 111;
	guiles_melody.timeUnit = 80000*(60000/guiles_melody.tempo/4);
	
	guiles_tuba.notes = notes_guiles_tuba;
	guiles_tuba.volumes = volumes_guile_tuba;
	guiles_tuba.tempo = 100;
	guiles_tuba.instr = &horn;
	guiles_tuba.length = 111;
	guiles_tuba.timeUnit = 80000*(60000/guiles_tuba.tempo/4);
	
	test_scale.notes = notes_scale;
	test_scale.volumes = volumes_scale;
	test_scale.tempo = 61;
	test_scale.instr = &oboe;
	test_scale.length = 15;
	test_scale.timeUnit = 80000*(60000/test_scale.tempo/4);
}
