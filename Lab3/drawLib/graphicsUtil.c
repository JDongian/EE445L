//Basic util functions to aid graphics
#include "graphicsUtil.h"
#include "draw2D.h"
#include "../ST7735.h"
#include <string.h>

#define BLACK 0x00
#define ENABLE_WRAP_AROUND


// buffer is initially not ready, black, at index 0.
volatile graphicsBuffer gbuff = {0};


char nextBufferIndex(char i) {
  return (i + 1) % 3;
}


////Buffer interface functions
void initBuffer() {
	gbuff.queued = -1;
}

void clearBuffer() {
  if (gbuff.full) { return; }

  // Set all pixels to black
  memset((uint16_t*)(gbuff.buffer[gbuff.rendering]), BLACK, SCREEN_WIDTH * SCREEN_HEIGHT);
}

// Set frameBuffer to a solid color
void setBuffer(uint16_t color) {
  if (gbuff.full) { return; }

  for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; ++i) {
    gbuff.buffer[gbuff.rendering][i] = color;
  }
}

void setPx(int x, int y, uint16_t color) {
  if (gbuff.full) { return; }

  #ifdef ENABLE_WRAP_AROUND
    x = ((x % SCREEN_WIDTH) + SCREEN_WIDTH) % SCREEN_WIDTH;
    y = ((y % SCREEN_HEIGHT) + SCREEN_HEIGHT) % SCREEN_HEIGHT;
  #endif /* ENABLE_WRAP_AROUND */

  gbuff.buffer[gbuff.rendering][xytoindex(x, y)] = color;
}

void finishBuffer() {
  if (gbuff.queued == -1) {
    gbuff.queued = gbuff.rendering;
    gbuff.rendering = nextBufferIndex(gbuff.rendering);
		clearBuffer();
  } else {
    gbuff.full = true;
  }
}


//// Rendering functions
void renderBuffer() {
  for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; ++i) {
    if (gbuff.buffer[gbuff.displayed][i] !=
        gbuff.buffer[gbuff.queued][i]) {
      ST7735_DrawPixel(indextox(i), indextoy(i),
                       gbuff.buffer[gbuff.queued][i]);
    }
  }
}

void attemptRender() {
  if (gbuff.queued != -1) {
    renderBuffer();
    gbuff.displayed = gbuff.queued;
    gbuff.queued = -1;
    gbuff.full = false;
  }
}
