/* jid295 Joshua Dong TODO: header */

#include <stdlib.h>
#include <stdint.h>
#include "ST7735.h"
#include "fixed.h"


void ST7735_printBuf(char buffer[], int length) {
  for (int i = 0; i < length; ++i) {
		ST7735_OutChar(buffer[i]);
	}
}

// TODO: move to drawing library
void drawPoint(point p, short color) {
    ST7735_DrawPixel(p.x, p.y, color);
}

// TODO: move to drawing library
point point_new(int x, int y) {
    point temp;
    temp.x = x;
    temp.y = y;
    return temp;
}

// TODO: move to drawing library
void ST7735_drawLine(point a, point b, short color) {
    int dx = abs(b.x - a.x), sx = a.x < b.x ? 1 : -1;
    int dy = abs(b.y - a.y), sy = a.y < b.y ? 1 : -1; 
    int err = (dx > dy ? dx : -dy)/2, e2; // two error calculations for line aliases
    while (a.x != b.x || a.y != b.y) {
        drawPoint(point_new(a.x, a.y), color);
        e2 = err;
        if (e2 > -dx) {
            err -= dy;
            a.x += sx;
        }
        if (e2 < dy) {
            err += dx;
            a.y += sy;
        }
    }
}

char get_lsd(uint32_t n) {
	return (n % 10) + '0';
}


void ST7735_sDecOut3(int32_t n)
{
  char buffer[6] = " *.***";
	
  if (-9999 <= n && n <= 9999) {
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
	
	ST7735_printBuf(buffer, 6);    
}


void ST7735_uBinOut8(uint32_t n)
{
  char buffer[6] = "  0.00";
	
  if (n < 256000) {
		n = n * 100 / 256;
    
    for (int i = 5; 0 <= i && 0 < n; --i) {
      if(i != 3) {
        buffer[i] = get_lsd(n);
        n /= 10;
      }
    }
  } else {
		//strncpy(buffer, "***.**", 6);
		for (int i = 0; i < 6; ++i) {
			if (i == 3) {
				buffer[i] = '.';
			} else {
			  buffer[i] = '*';
			}
		}
  }
  
	ST7735_printBuf(buffer, 6);
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
	
  //ST7735_FillScreen(0);  
  ST7735_SetCursor(0, 0);
  ST7735_SetTextColor(ST7735_WHITE); // arbitrary color
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
    int hwX = (127 * (x - MinX)) / (MaxX - MinX);  
    int hwY = 32 + (127 * (MaxY - y)) / (MaxY - MinY);
    ST7735_DrawPixel(hwX, hwY, ST7735_MAGENTA);
  }
}

void ST7735_PlotBar_Lab2(int32_t x, int32_t y){
	int32_t i, j;
  if (x < MinX || MaxX < x || y < MinY || MaxY < y) {
			// Neglect any points outside the minX maxY minY maxY bounds.
			return;
		}
  // X goes from 0 to 127
  // j goes from 159 to 32
  // y=Ymax maps to j=32
  // y=Ymin maps to j=159
	i = (127 * (x - MinX)) / (MaxX - MinX); 
  j = 32 + (127 * (MaxY - y)) / (MaxY - MinY);
  ST7735_DrawFastVLine(i, j, 159-j, ST7735_MAGENTA);

}

void drawPolygon(point* verticies, int numberOfVerticies, short shade) {
	int i;
	for(i = 0; i < numberOfVerticies-1; i++) {
		ST7735_drawLine(verticies[i], verticies[i+1], shade);
	} ST7735_drawLine(verticies[i], verticies[0], shade);
}

