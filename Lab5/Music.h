// Music.h

#define Off         0		
#define As2					117
#define C3					131
#define D3					147
#define Ds3					156
#define E3					165
#define F3					174
#define G3					196
#define Gs3					208
#define As3					233
#define C4          262
#define D4          294
#define Ds4					311
#define E4          330
#define F4          349
#define G4          392
#define A4          440
#define B4					494
#define C5          524
#define D5          588
#define E5          660
#define F5          698
#define G5          784
#define A5          880
#define B5					988

//Times
#define semibreve		16*semiquaver
#define minim				8*semiquaver
#define crotchet		4*semiquaver
#define quaver			2*semiquaver
#define semiquaver	10

typedef struct Note {
    uint16_t frequency;
    uint16_t time; // could be reduced
} Note;

typedef struct Music {
    note notes[1024];
    uint16_t durations[1024];
    uint8_t volumes[1024];
    uint16_t tempo;
    uint8_t waveform[8];
} Music;

typedef struct MusicIndex {
    uint8_t note;
    double time;
} MusicIndex;
