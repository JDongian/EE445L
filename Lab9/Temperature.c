#include <stdint.h>
#include "ST7735.h"
#include "fixed.h"
#include "calib.h"


#define FS 10000
#define N 128
#define DATA_GRANULARITY 53


uint16_t const ADCdata[DATA_GRANULARITY]={0,100,176,253,329,406,482,559,637,714,791,					
    869,947,1024,1102,1180,1258,1336,1415,1493,1571,					
    1649,1728,1806,1884,1963,2041,2119,2197,2275,2353,					
    2431,2509,2587,2664,2742,2819,2896,2973,3050,3126,					
    3202,3278,3354,3430,3505,3580,3655,3729,3803,3877,3950,4096};											

uint16_t const Tdata[DATA_GRANULARITY]={4000,4000,3920,3840,3760,3680,3600,3520,3440,3360,3280,		
    3200,3120,3040,2960,2880,2800,2720,2640,2560,2480,		
    2400,2320,2240,2160,2080,2000,1920,1840,1760,1680,		
    1600,1520,1440,1360,1280,1200,1120,1040,960,880,		
    800,720,640,560,480,400,320,240,160,80,0,0};			


uint16_t getTemp(uint16_t data)
{
    uint16_t temp = 0;

    for(int i = 0; i < DATA_GRANULARITY; i++){
        // find the ADC values for which the data value is between
        if (ADCdata[i] >= data) {
            uint16_t adcRange = ADCdata[i] - ADCdata[i-1];
            uint16_t tRange = Tdata[i-1] - Tdata[i];
            uint16_t adcOffset = data - ADCdata[i-1];
            uint16_t tOffset = tRange * (float)adcOffset / adcRange;
            temp = Tdata[i-1] - (float)tOffset;
            break;
        }
    }

    return temp + TEMP_OFFSET;
}


void ST7735_InitTemperatureGraph()
{
    ST7735_SetCursor(0,0); 
    ST7735_OutString("Temperature Data");
    ST7735_PlotClear(1000,4000);  // range from 0 to 4095
    ST7735_SetCursor(0,1); 
    ST7735_OutString("N=");
    ST7735_SetCursor(0,2); 
    ST7735_OutString("T= "); 
    ST7735_sDecOut2(2500);
    ST7735_OutString(" C");
}


void ST7735_UpdateTemperatureGraph(uint32_t numSamples, uint16_t adcValue)
{
    uint16_t temperature = getTemp(adcValue);
    
    // DEBUG
    // Magnify the plot to see the noise distribution better
    // ST7735_PlotPoint((temperature - 2200)*15);
    ST7735_PlotPoint(temperature);  // Measured temperature
    if((numSamples&(N-1))==0){          // fs sampling, fs/N samples plotted per second
        ST7735_PlotNextErase();  // overwrites N points on same line
    }
    if((numSamples%FS)==0){    // fs sampling, 1 Hz display of numerical data
        ST7735_SetCursor(3,1); 
        ST7735_OutUDec(adcValue);            // 0 to 4095
        ST7735_OutString(" "); // clear previous number
        ST7735_SetCursor(3,2); 
        ST7735_sDecOut2(temperature); // 0.01 C 
    }
}
