#include "Music.h"
#include "Wave.h"
#include "MAX5353.h"
#include "../inc/tm4c123gh6pm.h"
#include "Instrument.h"
#include "Song.h"

#include <stdbool.h>

// DEBUG LED
#define PF1       (*((volatile uint32_t *)0x40025008))

#define NUM_INSTRUMENTS 5

MusicIndex i = {0, 0};
MusicIndex i2 = {0,0};
Song* currentSong = &test_scale;
Song* currentSong2 = &guiles_tuba;
double envelopeIndex = 0;
double envelopeIndex2 = 0;
uint8_t instrIndex = 0;
uint8_t instrIndex2 = 0;
volatile uint16_t song1_value = 0;
volatile uint16_t song2_value = 0;


// an array of instruments allows one to toggle between instruments
Instr* instruments[NUM_INSTRUMENTS] = {
    &flute, 
    &horn, 
    &bassoon, 
    &oboe,
    &trumpet
};


// calculate the proportion of the current note played --
// this is used to envelope notes
double calc_proportion(bool isMelody)
{
    if (isMelody) {
        return TIMER1_TAILR_R*envelopeIndex/
            (currentSong->notes[i.noteIndex].time*currentSong->timeUnit);
    } else {
        return TIMER3_TAILR_R*envelopeIndex2/
            (currentSong2->notes[i2.noteIndex].time*currentSong2->timeUnit);
    }
}


// based on frequency, calculate appropriate timer isr
uint32_t calc_tailr(bool isMelody)
{
    if (isMelody) {
        return 80000000./
            currentSong->notes[i.noteIndex].frequency/INSTR_RES;
    } else {
        return 80000000./
            currentSong2->notes[i2.noteIndex].frequency/INSTR_RES;
    }
}

// calculate the required dac output values, to be summed later.
void PlayInstrument(double proportion, bool isMelody)
{
    static int waveIndex = 0;
    static int waveIndex2 = 0;

    if (isMelody) {
        DAC_OutValue(envelope((*currentSong->instr)[waveIndex], proportion));
        song1_value = envelope((*currentSong->instr)[waveIndex], proportion);
        waveIndex = (waveIndex + 1) % INSTR_RES;
    } else {
        //DAC_OutValue(envelope((*currentSong2->instr)[waveIndex2], proportion));
        song2_value = envelope((*currentSong2->instr)[waveIndex2], proportion);
        waveIndex2 = (waveIndex2 + 1) % INSTR_RES;
    }
}


void ChangeInstrument()
{
    currentSong->instr = instruments[instrIndex++ % NUM_INSTRUMENTS];
}

// advance the note index
void ChangeNote(bool isMelody)
{
    if (isMelody) {
        i.noteIndex = (i.noteIndex + 1) % currentSong->length;
        TIMER2_TAILR_R = currentSong->notes[i.noteIndex].time*currentSong->timeUnit;
        envelopeIndex = 0;
    } else {
        i2.noteIndex = (i2.noteIndex + 1) % currentSong2->length;
        TIMER4_TAILR_R = currentSong2->notes[i2.noteIndex].time*currentSong2->timeUnit;
        envelopeIndex2 = 0;
    }

    // toggle the LED on every note change
    PF1 ^= 0x02;
}

// entry point to play two songs
void PlayMusic(Song* s1, Song* s2){
    TIMER1_CTL_R = 0;
    TIMER2_CTL_R = 0;
    TIMER3_CTL_R = 0;
    TIMER4_CTL_R = 0;

    currentSong = s1;
    i.noteIndex = 0;
    TIMER1_TAILR_R  = calc_tailr(true);
    TIMER2_TAILR_R = currentSong->notes[i.noteIndex].time
        * currentSong->timeUnit;

    currentSong2 = s2;
    i2.noteIndex = 0;
    TIMER3_TAILR_R  = calc_tailr(false);
    TIMER4_TAILR_R = currentSong2->notes[i2.noteIndex].time
        * currentSong2->timeUnit;

    TIMER1_CTL_R = 1;
    TIMER2_CTL_R = 1;
    TIMER3_CTL_R = 1;
    TIMER4_CTL_R = 1;
}

// not used
void ChangeNote2() {
    i2.noteIndex = (i2.noteIndex+1) % currentSong2->length;
    uint16_t time = currentSong2->notes[i2.noteIndex].time;
    TIMER4_TAILR_R = time*currentSong2->timeUnit;
    envelopeIndex2 = 0;
    PF1 ^= 0x02;
}

// not used
void PlayMusic2(Song* s)
{
    TIMER3_CTL_R = 0;
    TIMER4_CTL_R = 0;
    currentSong2 = s;
    i2.noteIndex = 0;
    TIMER3_TAILR_R  = calc_tailr(false);
    TIMER4_TAILR_R = currentSong->notes[i.noteIndex].time*currentSong->timeUnit;
    TIMER3_CTL_R = 1;
    TIMER4_CTL_R = 1;
}

// waveform calculator (melody)
void Timer1A_Handler()
{
    TIMER1_ICR_R = TIMER_ICR_TATOCINT;
    PlayInstrument(calc_proportion(true), true);
    envelopeIndex++;
}

// note handler (melody)
void Timer2A_Handler()
{
    TIMER2_ICR_R = TIMER_ICR_TATOCINT;
    ChangeNote(true);
    TIMER1_TAILR_R = calc_tailr(true);
}

// waveform calculator (baseline)
void Timer3A_Handler()
{
    TIMER3_ICR_R = TIMER_ICR_TATOCINT;
    PlayInstrument(calc_proportion(false), false);
    envelopeIndex2++;
}

// note handler (baseline)
void Timer4A_Handler()
{
    TIMER4_ICR_R = TIMER_ICR_TATOCINT;
    ChangeNote(false);
    TIMER3_TAILR_R = calc_tailr(false);
}

// hardware output
void Timer5A_Handler()
{
    TIMER5_ICR_R = TIMER_ICR_TATOCINT;
    DAC_OutValue(song1_value + song2_value);
}
