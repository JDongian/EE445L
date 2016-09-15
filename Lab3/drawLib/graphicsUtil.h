#include "math2.h"
#include <stdint.h>
#include <stdbool.h>

#ifndef _SCREEN_DIM
#define _SCREEN_DIM
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 160
#endif /* SCREEN_DIM */

#define xytoindex(X, Y) (Y) * SCREEN_WIDTH + (X)
#define indextoy(I) (I) / SCREEN_WIDTH
#define indextox(I) (I) % SCREEN_WIDTH

/* Triple buffered graphics is setup to enable differential rendering.
 * If the status is TRUE, then the buffer at the current index is ready to
 * render. Otherwise, nothing needs to be done in hardware (no new frame or
 * frame is not ready). When frames are rendered, the buffer index advances
 * and status is set to FALSE.
 */
#ifndef _GBUFFER
#define _GBUFFER
typedef struct graphicsBuffer {
    uint16_t buffer[3][SCREEN_WIDTH * SCREEN_HEIGHT];
    char rendering;
    char queued;
    char displayed;
    bool full;
} graphicsBuffer;
#endif /* _GBUFFER */

//// Buffer interface functions
// Set up the buffer for the first time.
void initBuffer(void);

// Set current buffer to black.
void clearBuffer(void); 

// Sets current frameBuffer to a color.
void setBuffer(uint16_t color);

// Writes to the current frameBuffer.
void setPx(int x, int y, uint16_t color);

// Set the current buffer as complete.
void finishBuffer(void);


//// Rendering functions
void attemptRender(void);

