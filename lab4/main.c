#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"

int main()
{
	uint32_t ui32Period;

	// Set up the clock
	SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

	// GPIO configuration
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

	// Timer configuration
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0); // enable clock to TIMER0
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC); // Configure TIMER0 as 32 bit timer

	// Calculate and set delay
	ui32Period = (SysCtlClockGet() / 10) / 2; //
	TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period - 1);

	// Enable interrupt
	IntEnable(INT_TIMER0A); // enable vector associated with TIMER0A
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT); // Enable event to generate interrupt
	IntMasterEnable(); // Master int enable for all interrupts

	// Enable the timer
	TimerEnable(TIMER0_BASE, TIMER_A);

	while(1);
}

void Timer0IntHandler(void)
{
	// Clear the timer interrupt
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

	//Read the current state of the GPIOpin
	// and write backthe opposite state
	if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2))
	{
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0);
	}
	else
	{
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 4);
	}
}
