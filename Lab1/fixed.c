/* jid295 Joshua Dong TODO: header */

#include <stdlib.h>
#include <stdint.h>
#include "ST7735.h"
#define DEC_MIN -9999
#define DEC_MAX 9999


void ST7735_print_buf(char buffer[], int length) {
	// Print out a string of characters to the screen
  for (int i = 0; i < length; ++i) {
		ST7735_OutChar(buffer[i]);
	}
}


char get_lsd(uint32_t n) {
	// Get the least significant binary digit
	return (n % 10) + '0';
}


void ST7735_sDecOut3(int32_t n)
{
  char buffer[6] = " *.***";
	
  if (DEC_MIN <= n && n <= DEC_MAX) {
		// set the negative sign if n is negative
    if (n < 0) {
      buffer[0] = '-';
			n *= -1;
    }
    for (int i = 5; i > 0; --i) {
			if (i == 2) {
				continue; // Skip the decimal point.
			}
      buffer[i] = get_lsd(n);
      n /= 10;
    }
  }
	
	ST7735_print_buf(buffer, 6);    
}


void ST7735_uBinOut8(uint32_t n)
{
  char buffer[6] = "  0.00";
	
  if (n < 256000) {
		// Convert from binary to decimal with the 1/256 delta
		n = n * 100 / 256;
    
    for (int i = 5; 0 <= i && 0 < n; --i) {
      if(i != 3) {
        buffer[i] = get_lsd(n);
        n /= 10;
      }
    }
  } else {
		// If the value is out of range, write the special value to the buffer.
		//strncpy(buffer, "***.**", 6);
		for (int i = 0; i < 6; ++i) {
			if (i == 3) {
				buffer[i] = '.';
			} else {
			  buffer[i] = '*';
			}
		}
  }
  
	ST7735_print_buf(buffer, 6);
}


volatile int MinX, MaxX, MinY, MaxY;

void ST7735_XYplotInit(char *title, int32_t minX, int32_t maxX, int32_t minY, int32_t maxY)
{
  MinX = minX;
  MaxX = maxX;
  MinY = minY;
  MaxY = maxY;
	//assert(minX < maxX);
	//assert(minY < maxY);
	
  ST7735_FillScreen(0); // start with black
  ST7735_SetCursor(0, 0); // top right corner
  ST7735_SetTextColor(ST7735_WHITE); // arbitrary text color
  ST7735_OutString(title);
}


void ST7735_XYplot(uint32_t count, int32_t bufX[], int32_t bufY[])
{
  for (int i = 0; i < count; ++i) {
    int x = bufX[i];
    int y = bufY[i];
		
		if (x < MinX || MaxX < x || y < MinY || MaxY < y) {
			// Neglect any points outside the minX maxY minY maxY bounds.
			continue;
		}
		// from plotpoints
	// X goes from 0 to 127
  // j goes from 159 to 32
  // y=Ymax maps to j=32
  // y=Ymin maps to j=159
		
		// Scale the x and y values to the preset parameters
    int hwX = (127 * (x - MinX)) / (MaxX - MinX);
    int hwY = 32 + (127 * (MaxY - y)) / (MaxY - MinY);
		
    ST7735_DrawPixel(hwX, hwY, ST7735_MAGENTA);
  }
}
