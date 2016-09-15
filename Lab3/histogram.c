// filename ******** histogram.c ************** 
// Trevor Eggenberger - tae498
// Joshua Dong - jid295


#include <stdint.h>
#include <string.h>
#include "ST7735.h"
#include "fixed.h"
#include "histogram.h"

#define MAX_RECORDS 1000
#define NUM_ADC_OUTPUTS 4095

#define PLOT_MARGIN 10

volatile int32_t numOcurrences[NUM_ADC_OUTPUTS];

uint32_t calculateJitter(volatile uint32_t times[], uint16_t length) {
	uint32_t max = 0;
	uint32_t min = (uint32_t)-1; // max uint32_t value
	
	// assert more than 1 entry
	// finds maximum and minimum time differences
	for (int i = 0; i < length-1; ++i) {
		uint32_t diff = times[i+1] - times[i];
		if (diff < min) {
			min = diff;
		}
		if (diff > max) {
			max = diff;
		}
	}
	// jitter is the difference between maximum and minimum time difference
	return max - min;
}

void histogramify(volatile uint32_t data[], uint16_t length, histogram* h) {
	uint32_t min = data[0];
	uint32_t max = 0;
	uint16_t index;
	
	for (int i = 0; i < length; ++i) {
		// find minimum ADC value in data
		if (data[i] < min) {
			min = data[i];
		}
		// find maximum ADC value in data
		if (max < data[i]) {
			max = data[i];
		}
		// increment count for ADC data value
		index = data[i];
		numOcurrences[index]++;
	}
	//assert(min <= max)
	h->min = min;
	h->max = max;
}


void ST_drawData(volatile uint32_t data[], uint16_t length) {
	histogram adc_hist;
	histogramify(data, length, &adc_hist);
	
	// Initialize histogram plot with 10 pixel margins
	ST7735_XYplotInit("", adc_hist.min-PLOT_MARGIN, adc_hist.max+PLOT_MARGIN, 0, MAX_RECORDS);
	
	for (int i = adc_hist.min; i <= adc_hist.max; ++i) {
		ST7735_PlotBar_Lab2(i, numOcurrences[i]);
	}
}


void resetNumOcurrences(void){
	memset((void*)numOcurrences, 0, sizeof(int32_t)*NUM_ADC_OUTPUTS);
}
