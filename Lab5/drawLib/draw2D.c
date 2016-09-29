#include "draw2D.h"


//Vector drawing
void drawLine(point a, point b, uint16_t color) {
    // Bresenham's line algorithm
    int dx = abs(b.x - a.x), sx = a.x < b.x ? 1 : -1;
    int dy = abs(b.y-a.y), sy = a.y < b.y ? 1 : -1; 
    int err = (dx > dy ? dx : -dy) / 2, e2;
    while (a.x != b.x || a.y != b.y) {
        drawPoint(a.x, a.y, color);
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

void drawCircle(point center, int radius, uint16_t color) {
    int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;
    //Draw the axis points.
    drawPoint(center.x, center.y + radius, color);
    drawPoint(center.x, center.y - radius, color);
    drawPoint(center.x + radius, center.y, color);
    drawPoint(center.x - radius, center.y, color);
    while (x < y) {
        // ddF_x == 2 * x + 1;
        // ddF_y == -2 * y;
        // f == x*x + y*y - radius*radius + 2*x - y + 1;
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        drawPoint(center.x + x, center.y + y, color);
        drawPoint(center.x - x, center.y + y, color);
        drawPoint(center.x + x, center.y - y, color);
        drawPoint(center.x - x, center.y - y, color);
        drawPoint(center.x + y, center.y + x, color);
        drawPoint(center.x - y, center.y + x, color);
        drawPoint(center.x + y, center.y - x, color);
        drawPoint(center.x - y, center.y - x, color);
    }
}
/*
void drawPolygon(point* verticies, int numberOfVerticies, unsigned char shade) {
	int i;
	for(i = 0; i < numberOfVerticies-1; i++) {
		drawLine(verticies[i], verticies[i+1], shade);
	} drawLine(verticies[i], verticies[0], shade);
}
void drawFilledPolygon(point* verticies, int numberOfVerticies, unsigned char shade) {
	int x, y;
	box myBox;
	if(getSetting() == 0) { return; }
	myBox = getBox(verticies, numberOfVerticies);
	for(y = myBox.topL.y; y <= myBox.botR.y; y++){
		for(x = myBox.topL.x; x <= myBox.botR.x; x++){
			if(pointInPolygon(verticies, numberOfVerticies, point_new(x, y))) {
				drawPoint(point_new(x, y), shade);
			}
		}
	}
	drawPolygon(verticies, numberOfVerticies, 0xF);
}
*/

/* SAMPLE MAIN
int main() {
  // board setup
	...
	//
	
  int i = 0;
  while (1) {
    uint16_t colors[7] = {
   	  ST7735_BLUE,
      ST7735_RED,
      ST7735_GREEN,
      ST7735_CYAN,
      ST7735_MAGENTA,
   	  ST7735_YELLOW,
      ST7735_WHITE
    };
    i += 1;
   
    drawLine(point_new(1,1), point_new(i+i/3,i), colors[i%7]);
*/
