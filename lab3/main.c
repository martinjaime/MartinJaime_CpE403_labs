#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

// Part (a) of task 3: Change sequence. Sequence was reversed.
uint8_t ui8PinData = 8; // For changing sequence of LED blinking.
// Part (b) of task 3: Blink two LEDs at a time.
// uint8_t ui8PinData = 6; // For changing sequence of LED blinking.

int main() {
	// Set to use 16MHz clock divided by 5. Use PLL. with main oscillator is used.
	SysCtlClockSet(SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
	// Enable the peripheral for GPIO at port F.
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	// Set the pins 1 through 3 at port F as outputs.
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	//clock = SysCtlClockGet();
	while( 1 )
	{
		// Write enable the value of the mask of ui8PinData to the pins 1 - 3.
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, ui8PinData);
		// Delay of 2M loop cycles.
		SysCtlDelay(2000000);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x00);
		SysCtlDelay(2000000);

		// Part (a) of task 3: Change sequence. Sequence was reversed.
		if (ui8PinData == 2) ui8PinData = 8; else ui8PinData = ui8PinData / 2;

		// Part (b) of task 3: Blink two LEDs at a time.
		/*
		if (ui8PinData == 12)
					ui8PinData = 6;
		else if (ui8PinData == 10)
			ui8PinData = ui8PinData + 2;
		else
			ui8PinData = ui8PinData + 4; */
	}
}
