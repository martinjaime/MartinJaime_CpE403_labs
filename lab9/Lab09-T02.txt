/*
 * Martin Jaime
 * CpE 403
 * Lab 09
 * Task 02: Use cosine function with 1000 sample points and
 * determine the time for FPU operation.
 */
#include <stdint.h>
#include <stdbool.h>
#include <math.h> // Math functions, including sinf()
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/fpu.h" // Floating point unit support
#include "driverlib/sysctl.h"

#define TARGET_IS_BLIZZARD_RB1 	// Symbol to access the API's in ROM.
#include "driverlib/rom.h"

#ifndef M_PI
#define M_PI	3.14159265358979323846
#endif

#define SERIES_LENGTH 1000 // 1000 calulations

float gSeriesData[SERIES_LENGTH]; // Hold calculated data

int32_t i32DataCount = 0; // count of data to iterate

int main(void)
{
    float fRadians; // segment of 2pi to iterate by

    ROM_FPULazyStackingEnable(); 	// Turn on lazy stacking
    ROM_FPUEnable();				// Turn on FPU

    // Set up clock to 50 MHz 264262 - 296453
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    fRadians = ((2 * M_PI) / SERIES_LENGTH); // Get a segment of 2pi

    while(i32DataCount < SERIES_LENGTH) // Calculate 1000 values
    {
    	gSeriesData[i32DataCount] = cosf(fRadians * i32DataCount); // calculate sine value
    	i32DataCount++; // count up to get next sine value
    }

    while(1);
}
