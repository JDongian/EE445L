#include "Music.h"
#include "Wave.h"

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

void Music_Play(Music song) {
    MusicIndex i = {0, 0};
    while (1) {
        currentTime = 0.1;
        Wave_Value(flute, currentTime - i.time, GreenHills[i.note], 1);
    }
}
