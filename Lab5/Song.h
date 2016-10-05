#ifndef _SONG_
#define _SONG_

#include "Instrument.h"

typedef struct Note {
  uint16_t frequency;
  uint16_t time; // could be reduced
} Note;

typedef struct Song {
  Note* notes;
  uint8_t* volumes;
  uint16_t length;
  uint16_t tempo;
  Instr* instr;
  double timeUnit;
} Song;

extern Song greenhills;
extern Song test_repeat;
extern Song guiles_melody;
extern Song guiles_tuba;
extern Song test_scale;
//extern Song scale;


void init_songs(void);
#endif /* _SONG_ */

