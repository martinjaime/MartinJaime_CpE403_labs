/*
 * Martin Jaime
 * CpE 403
 * Lab 06
 * Task 01: Original code
 */
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/debug.h"
#include "driverlib/hibernate.h" // Hibernate modules
#include "driverlib/gpio.h"

#ifdef DEBUG
void__error__(char *pcFilename, uint32_t ui32Line)
{}
#endif

int main()
{
	// Set the clock to 40 MHz
	SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

	// Enable LEDs as outputs and turn on green LED.
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	//GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x08);

	// Enable hibernation module.
	SysCtlPeripheralEnable(SYSCTL_PERIPH_HIBERNATE);
	// Define frequency for clock for hibernation module.
	HibernateEnableExpClk(SysCtlClockGet());
	HibernateGPIORetentionEnable(); // continue GPIO state during hibernation.
	SysCtlDelay(64000000); // delay to allow user to observe the LED.

	// @TODO: After measuring current for step 17, uncomment next three lines
	// Configure RTC wakeup parameters
	HibernateRTCSet(0);			// reset RTC to 0
	HibernateRTCEnable();		// turn the RTC on and
	HibernateRTCMatchSet(0, 5); // set wake up time to 5 seconds after RTC = 0

	// @TODO: and use
	// Set wake_pin and RTC as wake condition
	HibernateWakeSet(HIBERNATE_WAKE_PIN | HIBERNATE_WAKE_RTC);

	// @TODO: Instead of this
	//HibernateWakeSet(HIBERNATE_WAKE_PIN); // set wake_pin as wake condition.
	//@TODO: uncomment next line too.
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x00); // turn off LED beefore going to sleep.

	HibernateRequest(); // request hibernation module to hibernate system.
	while(1);
}
