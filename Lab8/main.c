/*
 * main.c
 
 * derived from TI's getweather example
 * Copyright (C) 2014 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "utils/cmdline.h"
#include "application_commands.h"
#include "LED.h"
#include "Nokia5110.h"
#include "ST7735.h"
#include "ADCSWTrigger.h"
#include "../inc/tm4c123gh6pm.h"
//#include "Timers.h"
#include "PLL.h"
#include "motorLib/motor.h"
#include "MPU6050.h"
#include "i2c.h"
#include "SysTick.h"
#include "board.h"
#include "switch.h"


// DEBUG
typedef enum Mode {FWD, BWD, LFT, RGT} Mode;
Mode mode;


void UART_Init(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTStdioConfig(0, 115200, 50000000);
}


int main(void)
{
    initClk();       // set system clock to 50 MHz
    Init_Switches();
    motor_init();
    MPU6050_Init();
    UART_Init();
    //Init_Timers();
    motor_set(PORT, FORWARD, 1.0);
    motor_set(STARBOARD, FORWARD, 1.0);
    int data_x, data_y, data_z;
    uint16_t i = 0;

    while(1) {
        if (i == 0){
            data_x = MPU6050_ReadGyro('x');
            data_y = MPU6050_ReadGyro('y');
            data_z = MPU6050_ReadGyro('z');
            UARTprintf("gyroX: %d, gyroY: %d, gyroZ: %d\n\r", data_x, data_y, data_z);
        }
        i++;

        // set the mode based on the buttons (DEBUG)
        // handleButtons();
       
        // DEBUG
        // Switch demo
        /*
        switch (mode) {
            case FWD:
                motor_set(PORT, FORWARD, 1.0);
                motor_set(STARBOARD, FORWARD, 1.0);
                break;
            case BWD:
                motor_set(PORT, BACKWARD, 1.0);
                motor_set(STARBOARD, BACKWARD, 1.0);
                break;
            case LFT:
                motor_set(PORT, BACKWARD, 1.0);
                motor_set(STARBOARD, FORWARD, 1.0);
                break;
            case RGT:
                motor_set(PORT, FORWARD, 1.0);
                motor_set(STARBOARD, BACKWARD, 1.0);
                break;
        } 
        motor_run(0);
        */
    }
}

// timer0 {
        //motor_run(microseconds);
