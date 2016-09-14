//Basic util functions to aid graphics
#include "graphicsUtil.h"
#include "draw2D.h"

#define SCREEN_WIDTH 127
#define SCREEN_HEIGHT 64
#define PIXELS

uint16_t frameBuffer[SCREEN_WIDTH * SCREEN_HEIGHT] = {0};


////Buffer interface functions
uint16_t* getBuffer(void) { return frameBuffer; }

// Clear frameBuffer (faster due to memset?)
void clearBuffer() {
    memset(frameBuffer, 0, SCREEN_WIDTH * SCREEN_HEIGHT);
}

// Set frameBuffer to a solid color
void setBuffer(uint16_t color) {
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; ++i) {
        frameBuffer[i] = color;
    }
}

void setPx(int x, int y, uint16_t color) {
    #ifdef ENABLE_WRAP_AROUND
        x = ((x % SCREEN_WIDTH) + SCREEN_WIDTH) % SCREEN_WIDTH;
        y = ((y % SCREEN_HEIGHT) + SCREEN_HEIGHT) % SCREEN_HEIGHT;
    #endif /* ENABLE_WRAP_AROUND */
    frameBuffer[y * SCREEN_WIDTH + x] = value;
}
