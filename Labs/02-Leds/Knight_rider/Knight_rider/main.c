/* Defines -----------------------------------------------------------*/
#define LED_GREEN   PB5     // AVR pin where green LED is connected
#define LED_RED     PC0
#define BLINK_DELAY 100
#define Button PD0

#ifndef F_CPU
#define F_CPU 16000000      // CPU frequency in Hz required for delay
#endif

/* Includes ----------------------------------------------------------*/
#include <util/delay.h>     // Functions for busy-wait delay loops
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/sfr_defs.h>
/* Functions ---------------------------------------------------------*/
/**
* Main function where the program execution begins. Toggle two LEDs
* when a push button is pressed.
*/
int main(void)
{
	/* GREEN LED */
	// Set pin as output in Data Direction Register...
	/* second LED */
	// WRITE YOUR CODE HERE
	//init data register
	DDRC = 0xFF;
	PORTC=0xFF;

	// Button
	DDRB = DDRB | (0<<Button);
	PORTD=PORTD| (1<<Button);

	//
	PORTC=PORTC & ~(1<<0);
	// Infinite loop
	int i=0;
	int up=1;
	while (1)
	{
		// Pause several milliseconds

		// WRITE YOUR CODE HERE
		if (bit_is_clear(PIND,0) && up!=0) // if button pressed and direction is up
		{
		
			PORTC = PORTC ^ (1<<i);
			PORTC = PORTC ^ (1<<(i+1));
			i=i+1;
		}

		if (bit_is_clear(PIND,0) && up==0)
		{
			
			PORTC = PORTC ^ (1<<i);
			PORTC = PORTC ^ (1<<(i-1));
			i=i-1;
		}

		if(i==4 || i==0) //switch up/down directin
		up^=1;

		_delay_ms(BLINK_DELAY);
	}

	// Will never reach this
	return 0;
}