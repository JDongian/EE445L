#ifndef _SONG_
#define _SONG_

#include "Music.h"

typedef struct Song {
  Note (*notes)[1024];
  uint8_t (*volumes)[1024];
  uint16_t tempo;
  Instrument* instr;
} Song;

extern Song greenhills;
extern Song test_repeat;
//extern Song scale;

#endif /* _SONG_ */

