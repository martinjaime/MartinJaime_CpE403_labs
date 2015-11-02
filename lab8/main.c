/*
 * Martin Jaime
 * CpE 403
 * Lab 08
 * Original code
 */
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/flash.h"	// support for Flash APIs
#include "driverlib/eeprom.h" 	// support for EEPROM APIs

int main()
{
	// Buffer for read and write data and to initialize the write data.
	uint32_t pui32Data[2]; // For writing to memory.
	uint32_t pui32Read[2]; // For reading from memory.
	pui32Data[0] = 0x12345678;
	pui32Data[1] = 0x56789abc;

	// Set clock to 40 MHz
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	// Enable GPIO at port F with all pins with value of 0.
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00);

	SysCtlDelay(20000000); // Delay

	FlashErase(0x10000); // erase the block of flash starting at 0x10000
	FlashProgram(pui32Data, 0x10000, sizeof(pui32Data)); // Program pui32Data to the start of the block of length of pui32Data
	GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x02); // Light green LED
	SysCtlDelay(20000000); // Delay

	SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0); // Enable EEPROM peripheral
	EEPROMInit(); // Init EEPROM. Performs revocery if power failed during previous write op.
	EEPROMMassErase(); // unecessary. Erase entire EEPROM.
	EEPROMRead(pui32Read, 0x0, sizeof(pui32Read)); // Read EEPROM from 0 into pui32Read.
	EEPROMProgram(pui32Data, 0x0, sizeof(pui32Data));  // Write pui32Data values to beginning of EEPROM.
	EEPROMRead(pui32Read, 0x0, sizeof(pui32Read)); // Read values that were just written.
	// Turn off red LED, turn on blue LED
	GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x04);

	while(1);

}
