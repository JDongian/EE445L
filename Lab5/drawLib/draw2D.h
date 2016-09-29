#include "graphicsUtil.h"


//#define drawPoint(X, Y, C) setPx((X), (Y), (C))
#include "../ST7735.h"
#define drawPoint(X, Y, C) ST7735_DrawPixel((X), (Y), (C));

//Basic vector draw
void drawLine(point, point, uint16_t);   //Implements Bresenham's algorithm.
void drawCircle(point, int, uint16_t);
/*
void drawPolygon(point*, int, unsigned char);   //Connect the dots.
void drawFilledPolygon(point*, int, unsigned char); //Checks points in
																										//the fitting box for pairity.
//Special vector draw
								unsigned int, unsigned int);
void drawString(unsigned char*, point);
void drawNumber(int, point);
	//Warning: draws 999 max
*/
