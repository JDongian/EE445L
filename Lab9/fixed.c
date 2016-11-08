// filename ******** fixed.c ************** 
// Lab 1 c file
// Trevor Eggenberger - tae498
// Joshua Dong
// 8/31/16

#include <stdint.h>
#include "ST7735.h"

static int xMax, xMin, yMax, yMin;

char get_lsd(uint32_t n) {
	// Get the least significant binary digit
	return (n % 10) + '0';
}

void ST7735_print_buf(char buffer[], int length) {
	// Print out a string of characters to the screen
  for (int i = 0; i < length; ++i) {
		ST7735_OutChar(buffer[i]);
	}
}

/****************ST7735_sDecOut3***************
 converts fixed point number to LCD
 format signed 32-bit with resolution 0.001
 range -9.999 to +9.999
 Inputs:  signed 32-bit integer part of fixed-point number
 Outputs: none
 send exactly 6 characters to the LCD 
Parameter LCD display
 12345    " *.***"
  2345    " 2.345"  
 -8100    "-8.100"
  -102    "-0.102" 
    31    " 0.031" 
-12345    " *.***"
 */ 
void ST7735_sDecOut3(int32_t n)
{
	if (n > 9999 || n < -9999) {
		ST7735_OutString(" *.***");
		return;
	}
	if (n < 0) {
		ST7735_OutChar('-'); // output negative sign
		n *= -1; // make number positive for easier output
	} 
	else {
		ST7735_OutChar(' '); //number is positive -> output a space
	}
	int divider = 1000;
	int numDigit;
	// 5 iterations for 4 digits plus the decimal point
	for (int i = 0; i < 5; i++) { 
		if (i == 1) {
			ST7735_OutChar('.'); // output decimal point after first digit
		}
		else{
			numDigit = n/divider; // isolate the digit
			numDigit += 0x30; // convert to ASCII
			ST7735_OutChar(numDigit); //output digit
			n = n % divider; // get remainder to move to next digit
			divider /= 10; // change divider for next digit
		}
	}
}

void ST7735_sDecOut2(uint16_t n){
	char buffer[5] = "**.**";
	
    for (int i = 4; i >= 0; --i) {
			if (i == 2) {
				continue; // Skip the decimal point.
			}
      buffer[i] = get_lsd(n);
      n /= 10;
    }
	ST7735_print_buf(buffer, 5);    
}


/**************ST7735_uBinOut8***************
 unsigned 32-bit binary fixed-point with a resolution of 1/256. 
 The full-scale range is from 0 to 999.99. 
 If the integer part is larger than 256000, it signifies an error. 
 The ST7735_uBinOut8 function takes an unsigned 32-bit integer part 
 of the binary fixed-point number and outputs the fixed-point value on the LCD
 Inputs:  unsigned 32-bit integer part of binary fixed-point number
 Outputs: none
 send exactly 6 characters to the LCD 
Parameter LCD display
     0	  "  0.00"
     2	  "  0.01"
    64	  "  0.25"
   100	  "  0.39"
   500	  "  1.95"
   512	  "  2.00"
  5000	  " 19.53"
 30000	  "117.19"
255997	  "999.99"
256000	  "***.**"
*/
void ST7735_uBinOut8(uint32_t n)
{
	if (n >= 256000){
		ST7735_OutString("***.**");
		return;
	}
	
	uint32_t numDigit;
	
	if (n < 256){
		ST7735_OutChar(' ');
		ST7735_OutChar(' ');
		ST7735_OutChar('0');
		ST7735_OutChar('.');
	}
	else if (n >= 256 && n < 2560){
		ST7735_OutChar(' ');
		ST7735_OutChar(' ');
		numDigit = n / 256;
		ST7735_OutChar(numDigit + '0');
		ST7735_OutChar('.');
	}
	else if (n >= 2560 && n < 25600) {
		ST7735_OutChar(' ');
		numDigit = n / 256;
		int tensPlace = numDigit / 10;
		ST7735_OutChar(tensPlace + 0x30);
		
		int onesPlace = numDigit % 10;
		ST7735_OutChar(onesPlace + 0x30);
		
		ST7735_OutChar('.');
	}
	else {
		numDigit = n / 256;
		int hundredsPlace = numDigit / 100;
		ST7735_OutChar(hundredsPlace + 0x30);
		
		int tensPlace = (numDigit % 100)/10;
		ST7735_OutChar(tensPlace + 0x30);
		
		int onesPlace = (numDigit % 100) % 10;
		ST7735_OutChar(onesPlace + 0x30);
		
		ST7735_OutChar('.');
	}
	n %= 256;
	n *= 10;
	numDigit = n/256;
	ST7735_OutChar(numDigit + 0x30);
	n %= 256;
	n *= 10;
	numDigit = n/256;
	n %= 256;
	n *= 10;
	int roundDigit = n /256;
	if (roundDigit > 4) {
		numDigit += 1;
	}
	ST7735_OutChar(numDigit + 0x30);
	
}	

/**************ST7735_XYplotInit***************
 Specify the X and Y axes for an x-y scatter plot
 Draw the title and clear the plot area
 Inputs:  title  ASCII string to label the plot, null-termination
          minX   smallest X data value allowed, resolution= 0.001
          maxX   largest X data value allowed, resolution= 0.001
          minY   smallest Y data value allowed, resolution= 0.001
          maxY   largest Y data value allowed, resolution= 0.001
 Outputs: none
 assumes minX < maxX, and miny < maxY
*/
void ST7735_XYplotInit(char *title, int32_t minX, int32_t maxX, int32_t minY, int32_t maxY)
{
	ST7735_FillScreen(0);  // set screen to black
	ST7735_SetCursor(0,0);
	ST7735_OutString(title);
  ST7735_PlotClear(minY, maxY);
	yMin = minY;
	xMin = minX;
	yMax = maxY;
	xMax = maxX;
}

/**************ST7735_XYplot***************
 Plot an array of (x,y) data
 Inputs:  num    number of data points in the two arrays
          bufX   array of 32-bit fixed-point data, resolution= 0.001
          bufY   array of 32-bit fixed-point data, resolution= 0.001
 Outputs: none
 assumes ST7735_XYplotInit has been previously called
 neglect any points outside the minX maxY minY maxY bounds
*/
void ST7735_XYplot(uint32_t num, int32_t bufX[], int32_t bufY[])
{
	for (int i = 0; i < num; i++){
		int y = bufY[i];
		int x = bufX[i];
		if(y<yMin) y=yMin;
		if(y>yMax) y=yMax;
		// i goes from 0 to 127
		// j goes from 159 to 32
		// y=yMax maps to j=32
		// y=yMin maps to j=159
		int j = 32+(127*(yMax-y))/(yMax-yMin);
		int i = (127*(x-xMin))/(xMax - xMin);
		if(j<32) j = 32;
		if(j>159) j = 159;
		ST7735_DrawPixel(i,   j,   ST7735_BLUE);
		ST7735_DrawPixel(i+1, j,   ST7735_BLUE);
		ST7735_DrawPixel(i,   j+1, ST7735_BLUE);
		ST7735_DrawPixel(i+1, j+1, ST7735_BLUE);
	}
}


