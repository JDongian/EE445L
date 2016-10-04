#include "Music.h"
#include "Wave.h"
#include "MAX5353.h"
#include "../inc/tm4c123gh6pm.h"

#define PF1       (*((volatile uint32_t *)0x40025008))

double sysTime = 0;
MusicIndex m_i = {0, 0};
volatile uint32_t reload = 0;
volatile double ticks = 0;
double envelope_index = 0;


void play_instr(Instrument instr, double proportion)
{
  static int waveIndex = 0;

  DAC_OutValue(envelope(instr[waveIndex], proportion));

  waveIndex = (waveIndex + 1) % INSTR_RES;
}


void next_note(Song* song)
{
  m_i.noteIndex = (m_i.noteIndex + 1) % 60;
  uint16_t time = song->notes[m_i.noteIndex].time;
  reload = time * TIME_UNIT * 80000;

  // debug -- toggle the LED every time the note is advanced
  PF1 ^= 0x02;
}


long calc_tailr(Song* song, uint8_t index)
{
  return 80000000. / song->notes[index].frequency / INSTR_RES;
}


double calc_prop(long tailr, Song* song, uint8_t index)
{
  return tailr * envelope_index /
      (song->notes[index].time * TIME_UNIT * 80000);
}


Song current_song = greenhills;


void Timer1A_Handler()
{
  TIMER1_ICR_R = TIMER_ICR_TATOCINT;
  
  // calculate the relative position in the note currently at --
  // this is used for enveloping the note
  double proportion = calc_prop(TIMER1_TAILR_R, &current_song, m_i.noteIndex);
  play_instr(flute, proportion);
  
  envelope_index++;
}


void Timer2A_Handler()
{
  TIMER2_ICR_R = TIMER_ICR_TATOCINT;
  
  // set the frequency of the waveform
  TIMER1_TAILR_R = calc_tailr(&current_song, m_i.noteIndex);
  // reset the envelope index
  envelope_index = 0;
  
  // update note value
  next_note(&current_song);
  // trigger again after this note finishes
  TIMER2_TAILR_R = reload;
}

