// Switch.h
// Runs on LM4F120/TM4C123
// Low level drivers for the Switch

#ifndef _Switch
#define _Switch

//********Switch_Init*****************
// Initialization for Switch hardware.
// Input: none
// Output: none
void Switch_Init();

//********Switch_PressedPlay*****************
// inputs: none
// outputs: TRUE if the play/pause button was pressed
bool Switch_PressedPlay();

//********Switch_PressedReset*****************
// inputs: none
// outputs: TRUE if the reset button was pressed
bool Switch_PressedReset();

//********Switch_PressedMode*****************
// inputs: none
// outputs: TRUE if the mode button was pressed
bool Switch_PressedMode();

#endif /* _Switch */
