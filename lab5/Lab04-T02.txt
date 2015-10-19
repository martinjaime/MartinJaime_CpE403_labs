/*
 * Author: Martin Jaime
 * CpE 403: Lab 5
 * Task 2: Change the ADC Sequencer to SS2. Turn on the LED at PF1 if
 * 		   the temperature is greater than 80 deg-F.
 */
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"  // definitions to use ADC driver
#include "driverlib/gpio.h"

#define TARGET_IS_BLIZZARD_RB1 // Symbol to access the API's in ROM.
#include "driverlib/rom.h"

#ifdef DEBUG  // This code is executed when driver library encounters an error
void__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

int main()
{

	uint32_t ui32ADC0Value[4]; // ADC FIFO

	volatile uint32_t ui32TempAvg; // Store average
	volatile uint32_t ui32TempValueC; // Temp in C
	volatile uint32_t ui32TempValueF; // Temp in F

	// Set up the clock to 40 MHz
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

	// GPIO configuration
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); // enable port F
	// Set LEDs as outputs
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

	ROM_SysCtlPeripheralEnable( SYSCTL_PERIPH_ADC0 ); // enable the ADC0 peripheral
	ROM_ADCHardwareOversampleConfigure( ADC0_BASE, 64 ); // hardware averaging (64 samples)

	// Configure ADC0 sequencer to use sample sequencer 2, and have the processor trigger the sequence.
	ROM_ADCSequenceConfigure( ADC0_BASE, 2, ADC_TRIGGER_PROCESSOR, 0 );
	// Configure each step.
	ROM_ADCSequenceStepConfigure( ADC0_BASE, 2, 0, ADC_CTL_TS );
	ROM_ADCSequenceStepConfigure( ADC0_BASE, 2, 1, ADC_CTL_TS );
	ROM_ADCSequenceStepConfigure( ADC0_BASE, 2, 2, ADC_CTL_TS );
	// Sample temp sensor.
	ROM_ADCSequenceStepConfigure( ADC0_BASE, 2, 3, ADC_CTL_TS | ADC_CTL_IE | ADC_CTL_END);
	ROM_ADCSequenceEnable( ADC0_BASE, 2 ); // Enable ADC sequencer 2

	while(1)
	{
		ROM_ADCIntClear(ADC0_BASE, 2); // Clear ADC0 interrupt flag.
		ROM_ADCProcessorTrigger(ADC0_BASE, 2); // Trigger ADC conversion.

		while(!ROM_ADCIntStatus(ADC0_BASE, 2, false)); // wait for conversion to complete.

		ADCSequenceDataGet(ADC0_BASE, 2, ui32ADC0Value); // get converted data.
		// Average read values, and round.
		// Each Value in the array is the result of the mean of 64 samples.
		ui32TempAvg = (ui32ADC0Value[0] + ui32ADC0Value[1] + ui32ADC0Value[2] + ui32ADC0Value[3] + 2) / 4;
		ui32TempValueC = (1475 - ((2475 * ui32TempAvg)) / 4096) / 10; // calc temp in C
		ui32TempValueF = (( ui32TempValueC * 9) + 160)/5;

		// Read the current temperature. Light LED 1 if temp > 80 deg-F
		if (ui32TempValueF > 80)
		{
			ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 2);
		}
		else
		{
			ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0);
		}
	}
}
