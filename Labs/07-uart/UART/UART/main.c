/***********************************************************************
 * 
 * Analog-to-digital conversion with displaying result on LCD and 
 * transmitting via UART.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2018-2020 Tomas Fryza
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
#include "uart.h"           // Peter Fleury's UART library
#define F_CPU 16000000
/* Function definitions ----------------------------------------------*/
/**
 * Main function where the program execution begins. Use Timer/Counter1
 * and start ADC conversion four times per second. Send value to LCD
 * and UART.
 */
int main(void)
{
    // Initialize LCD display
    lcd_init(LCD_DISP_ON);
    lcd_gotoxy(1, 0); lcd_puts("value:");
    lcd_gotoxy(3, 1); lcd_puts("key:");
    lcd_gotoxy(8, 0); lcd_puts("a");    // Put ADC value in decimal
    lcd_gotoxy(13,0); lcd_puts("b");    // Put ADC value in hexadecimal
    lcd_gotoxy(8, 1); lcd_puts("c");    // Put button name here
	lcd_gotoxy(14,1); lcd_puts("P");
    // Configure ADC to convert PC0[A0] analog value
    // Set ADC reference to AVcc
	ADMUX= ADMUX | (1<<REFS0);
    // Set input channel to ADC0
	ADMUX =ADMUX & 0xF0;
    // Enable ADC module
	ADCSRA=ADCSRA |(1<<ADEN);
    // Enable conversion complete interrupt
	//ADCSRA=ADCSRA |(1<<ADIE);
    // Set clock prescaler to 128 and enable interrupt
	ADCSRA=ADCSRA | 0x0F;

    // Configure 16-bit Timer/Counter1 to start ADC conversion
    // Enable interrupt and set the overflow prescaler to 262 ms

	TIM1_overflow_262ms();
	TIM1_overflow_interrupt_enable();

    // Initialize UART to asynchronous, 8N1, 9600
	uart_init(UART_BAUD_SELECT(9600,F_CPU));

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
 * ISR starts when Timer/Counter1 overflows. Use single conversion mode
 * and start conversion four times per second.
 */
ISR(TIMER1_OVF_vect)
{
   ADCSRA=ADCSRA |(1<<ADSC);

}

/* -------------------------------------------------------------------*/
/**
 * ISR starts when ADC completes the conversion. Display value on LCD
 * and send it to UART.
 */
ISR(ADC_vect)
{
   uint16_t value = 0;
   char lcd_string[4] = "0000";
	
	
   value = ADC;						// Copy ADC result to 16-bit variable
   itoa(value, lcd_string, 10);		// Convert to string in decimal
	lcd_gotoxy(8, 0);				// set cursor to position 'a'
	lcd_puts("    ");				// clear space for new number to avoid remainder of the previous one when the new one has less decimal places
	lcd_gotoxy(8, 0);				// set cursor to position 'a'
	lcd_puts(lcd_string);			// send string 
	
	uart_puts("Button pressed: ");	//send text preceding value
	uart_puts(lcd_string);			// send character string over UART (value of adc)
	uart_puts("\n");				// end line
	
	lcd_gotoxy(13, 0);				// set cursor to position 'b'
	lcd_puts("   ");				// clear space for new number to avoid remainder of the previous one when the new one has less decimal places
	lcd_gotoxy(13, 0);				// set cursor to position 'b'
	
	itoa(value, lcd_string, 16);    // Convert to string in hex
	lcd_puts(lcd_string);			// send string
	
	
	// code for printing button name pressed 
	if(value<50)
	{
		lcd_gotoxy(8, 1);
		lcd_puts("      ");
		lcd_gotoxy(8, 1);
		lcd_puts("Right");
	}
	else if((value>=50) & (value < 170))
	{
		lcd_gotoxy(8, 1);
		lcd_puts("      ");
		lcd_gotoxy(8, 1);
		lcd_puts("Up");
	}
	else if((value>=170) & (value < 350))
	{
		lcd_gotoxy(8, 1);
		lcd_puts("      ");
		lcd_gotoxy(8, 1);
		lcd_puts("Down");
	}
	else if((value>=350) & (value < 500))
	{
		lcd_gotoxy(8, 1);
		lcd_puts("      ");
		lcd_gotoxy(8, 1);
		lcd_puts("Left");
	}
	else if((value>=500) & (value < 800))
	{
		lcd_gotoxy(8, 1);
		lcd_puts("      ");
		lcd_gotoxy(8, 1);
		lcd_puts("Select");
	}
	else
	{
		lcd_gotoxy(8, 1);
		lcd_puts("      ");
		lcd_gotoxy(8, 1);
		lcd_puts("None");
	}
	
	
	// parity bit computation for even parity
	
	uint8_t parity=0;
	
	for(uint8_t i=0;i<16;i++)		// go through the whole length of value, for UART this would be length of each data packet
	{
		parity^= (value & 0x01);	// parity XOR (if last bit is one)
		value>>=1;					// bit shift value right
	}
	
	lcd_gotoxy(15, 1);				// move to some free location
	if(parity==1)					// decide if parity bit is zero or one
	lcd_putc('1');					// put parity bit onto display
	else
	lcd_putc('0');					// put parity bit onto display
	
	
	
}