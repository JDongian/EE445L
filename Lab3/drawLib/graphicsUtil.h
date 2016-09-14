#include "hw_types.h"
#include "math2.h"


//Buffer interface functions
uint16_t* getBuffer(void);   //Returns the current frameBuffer.
void clearBuffer(void);   //Sets frameBuffer to black.
void setBuffer(uint16_t);   //Sets frameBuffer to shade.
void setPx(int, int, uint16_t);   //Writes to the frameBuffer.
