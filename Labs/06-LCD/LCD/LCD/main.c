/***********************************************************************
 *
 * Stopwatch with LCD display output.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2017-2020 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 *
 **********************************************************************/

 /* Includes ----------------------------------------------------------*/

#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include "lcd.h"            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for conversion function

/* Function definitions ----------------------------------------------*/
/**
 * Main function where the program execution begins. Update stopwatch
 * value on LCD display when 8-bit Timer/Counter2 overflows.
 */
int main(void)
{


	uint8_t one_bar[8] = {
		0b10000,
		0b10000,
		0b10000,
		0b10000,
		0b10000,
		0b10000,
		0b10000,
		0b10000
	};

	uint8_t two_bar[8] = {
		0b11000,
		0b11000,
		0b11000,
		0b11000,
		0b11000,
		0b11000,
		0b11000,
		0b11000
	};

	uint8_t three_bar[8] = {
		0b11100,
		0b11100,
		0b11100,
		0b11100,
		0b11100,
		0b11100,
		0b11100,
		0b11100
	};
	uint8_t four_bar[8] = {
		0b11110,
		0b11110,
		0b11110,
		0b11110,
		0b11110,
		0b11110,
		0b11110,
		0b11110
	};

	uint8_t five_bar[8] = {
		0b11111,
		0b11111,
		0b11111,
		0b11111,
		0b11111,
		0b11111,
		0b11111,
		0b11111
	};





	// Initialize LCD display
	lcd_init(LCD_DISP_ON);

	lcd_command(1 << LCD_CGRAM);
	for (uint8_t i = 0; i < 8; i++)
	{
		// Store all new chars to memory line by line
		lcd_data(one_bar[i]);
	}
	for (uint8_t i = 0; i < 8; i++)
	{
		// Store all new chars to memory line by line
		lcd_data(two_bar[i]);
	}
	for (uint8_t i = 0; i < 8; i++)
	{
		// Store all new chars to memory line by line
		lcd_data(three_bar[i]);
	}
	for (uint8_t i = 0; i < 8; i++)
	{
		// Store all new chars to memory line by line
		lcd_data(four_bar[i]);
	}
	for (uint8_t i = 0; i < 8; i++)
	{
		// Store all new chars to memory line by line
		lcd_data(five_bar[i]);
	}



	// Set DDRAM address
	lcd_command(1 << LCD_DDRAM);




	// Display first custom character
   // lcd_putc(0);


   // Put string(s) at LCD display
	lcd_gotoxy(1, 0);
	lcd_puts("00:00.0");
	lcd_gotoxy(11, 0);
	lcd_putc('a');

	lcd_gotoxy(1, 1);
	lcd_putc('b');

	lcd_gotoxy(11, 1);
	lcd_putc('c');

	// Configure 8-bit Timer/Counter2 for Stopwatch
	// Set prescaler and enable overflow interrupt every 16 ms
	TIM2_overflow_interrupt_enable();
	TIM2_overflow_16m();


	// Config timer0 16m ovf and enable
	TIM0_overflow_interrupt_enable();
	TIM0_overflow_16m();

	// Enables interrupts by setting the global interrupt mask
	sei();

	// Infinite loop
	while (1)
	{
		/* Empty loop. All subsequent operations are performed exclusively
		 * inside interrupt service routines ISRs */
	}

	// Will never reach this
	return 0;
}

/* Interrupt service routines ----------------------------------------*/
/**
 * ISR starts when Timer/Counter2 overflows. Update the stopwatch on
 * LCD display every sixth overflow, ie approximately every 100 ms
 * (6 x 16 ms = 100 ms).
 */
ISR(TIMER2_OVF_vect)
{
	static uint8_t number_of_overflows = 0;
	static uint8_t tens = 0;        // Tenths of a second
	static uint8_t secs = 0;        // Seconds
	static uint8_t minutes = 0;		// Minutes
	char lcd_string[2] = "  ";      // String for converting numbers by itoa()
	int sq = 0;

	number_of_overflows++;
	if (number_of_overflows >= 6)
	{
		// Do this every 6 x 16 ms = 100 ms
		number_of_overflows = 0;

		if (tens < 9)
			tens++;
		else
		{
			secs++;

			tens = 0;
		}

		if (secs > 59)
		{
			secs = 0;
			minutes++;
		}

		if (minutes > 59)
		{
			minutes = 0;
		}
		lcd_gotoxy(1, 0);
		if (minutes < 10) // if less than 10 minutes write to second position in seconds
		{

			lcd_putc('0');
			lcd_gotoxy(2, 0);

		}

		itoa(minutes, lcd_string, 10); // convert and output minutes
		lcd_puts(lcd_string);




		lcd_gotoxy(4, 0);

		if (secs < 10) // if less than 10 seconds write to second position in seconds
		{

			lcd_putc('0');
			lcd_gotoxy(5, 0);

		}


		itoa(secs, lcd_string, 10); // convert and ouput seconds
		lcd_puts(lcd_string);


		lcd_gotoxy(7, 0);
		itoa(tens, lcd_string, 10); // convert and output tenths of seconds
		lcd_puts(lcd_string);

		sq = (int)secs * (int)secs; // create square of secs and output onto display
		lcd_gotoxy(11, 0); 
			itoa(sq, lcd_string, 10);
		lcd_puts(lcd_string);
	}
}

ISR(TIMER0_OVF_vect)
{
	static uint8_t ovf = 0;
	static uint8_t symbol = 0;
	static uint8_t position = 0;

	static uint8_t ovf2 = 1;

	ovf++;

	if ((20 * (int)ovf2 - 16 * (int)ovf) < 0) // basically a rounding algorithm to change symbol / position every approx 20 ms
	{
		ovf2++;

		if (symbol < 4)
			symbol++;
		else
		{
			symbol = 0;
			position++;
		}
	}




	if (position > 9 || ovf >= 60) // if last position or overflow (to compensate for rounding) 
	{
		position = 0;
		symbol = 0;
		for (uint8_t i = 0; i <= 9; i++) // clear bar
		{
			lcd_gotoxy(1 + i, 1);
			lcd_putc(10);
		}
		ovf = 0;
		ovf2 = 1;
	}
	else
	{
		lcd_gotoxy(1 + position, 1);
		lcd_putc(symbol);

	}

}