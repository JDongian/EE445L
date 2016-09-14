// filename ******** histogram.h ************** 
// Trevor Eggenberger - tae498
// Joshua Dong - jid295

#include <stdint.h>


// used to store histogram dimensions
typedef struct histogram {
	uint32_t min;
	uint32_t max;
} histogram;

// calculates jitter in times array 
uint32_t calculateJitter(volatile uint32_t times[], uint16_t length);

/* finds max and min values in data array and counts 
 * number of occurrences of each value
 */
void histogramify(volatile uint32_t data[], uint16_t length, histogram* h);

// draws histogram using given data array
void ST_drawData(volatile uint32_t data[], uint16_t length);

// resets the numOcurrences array to 0 for next run
void resetNumOcurrences(void);
