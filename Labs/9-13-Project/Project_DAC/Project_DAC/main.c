/*
* Project_DAC.c
*
* Created: 18.11.2020 13:47:49
* Author : jira
*/

/**
* @ brief F_CPU
* @ warning Should Correspod with CPU frequency
*/
#define  F_CPU 16000000


#include <avr/io.h>			// include IO operation setting
//#include <math.h>			// Include math library for signal generation
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include "Sig_gen.h"
#include "lcd.h"
#include <stdlib.h>
#include <string.h>
#include "uart.h"



/** @brief  Decodes the selected button from butt variable
*/
void decode_button();



/** @brief  Changes the size of an array appropriatel to period, can switch timer ovf period if required (if the period does not fit within the interval, not used currently)
*/
void change_size();

uint8_t frame_buffer[1000];								//create frame buffer for outputting waveforms

volatile uint16_t frequency = 161;						// generated frequency in Hz
volatile uint8_t multiplier=1;							// multiplier for DTMF signal
uint16_t butt = 0;										// button pressed

uint16_t arr_size = 0;									// active size of frame buffer (frequency dependent)

float tim_set = 16e-6;									// inital timer set to 16 us

const float timer_values[] = { 16e-6,128e-6,1e-3 };		// settable timer ovf values

int main(void)
{

	change_size();
	lcd_init(LCD_DISP_ON);
	uart_init(UART_BAUD_SELECT(9600,F_CPU));
	update_disp(11,frequency,multiplier);
	DDRD = 0xFF;											// set D as output
	DDRB = 0xFF;											// set B as output
	DDRC = 0x07;											// set necesary C pins as output

	generate_signal(frame_buffer, 1, frequency, tim_set,multiplier);	// init frame buffer with sine wave of frequency 1



	TIM0_overflow_16u();
	TIM0_overflow_interrupt_enable();

	TIM2_overflow_1m();
	TIM2_overflow_interrupt_enable();





	sei();													// Enable interrupt

	
	while (1)
	{

	}
}


/** @brief  Frame buffer update timer (main sampling clock generator) moves in frame buffer
*/
ISR(TIMER0_OVF_vect)
{
	static uint16_t index = 0;

	


	if (index < arr_size -1)					//  if at end of active array portion move to start
	{
		
		index++;
	}
	else
	{
		
		index=0;								// clear index 2cyc
		
		asm("STS 0x0124,R1");					// balancing branches 2cyc
		asm("STS 0x0124,R1");					// balancing branches 2cyc
		
	}

	PORTB = frame_buffer[index];				// output the data

}

ISR(TIMER2_OVF_vect)
{
	
	static uint8_t ind = 0;						// indexing variable in columns
	uint16_t temp = 0;							// temporary variable for extracting button pressed
	static uint16_t debounce_reg = 0;			// debouncing register (stores value and outputs after desired period)
	
	static uint8_t ovf = 0;
	uint16_t  debounce_temp = debounce_reg;		// temporary debouncer to check if values have changed

	ind++;

	if (ind < 3)								// sweep through columns
	{

		PORTC &= 0xF8;
		PORTC |= (1 << ind);

	}
	else
	{

		ind = 0;
		PORTC &= 0xF8;
		PORTC |= (1 << ind);
	}

	switch (ind)								// extract button pressed and update debounce register
	{
		case 0:
		temp = PINC & 0b01111000;				// take the connected pins to 4x3 keypad
		temp = temp >> 3;						// shift to the beggining
		debounce_reg &= 0xFFF0;					// update the debounce register (zero out the current nibble)
		debounce_reg |= temp;					// update the current nibble
		break;

		case 1:
		temp = PINC & 0b01111000;
		temp = temp << 1;
		debounce_reg &= 0xFF0F;
		debounce_reg |= temp;
		break;

		case 2:
		temp = PINC & 0b01111000;
		temp = temp << 5;
		debounce_reg &= 0xF0FF;
		debounce_reg |= temp;
		break;


	}

	ovf++;

	if (ovf > 50 && debounce_reg == debounce_temp) // output debounced values if time has come and button values are stable
	{
		butt = debounce_reg;
		ovf = 0;
		decode_button();

	}




}


void decode_button(void)									// decodes which button has been pressed and updates variables the button changes
{
	uint8_t temp = 0;
	static uint8_t type = 11;								//default setting (on start generate sinewave)
	static uint8_t Func =0;
	static uint8_t accum=0;									//accum variable for variable speed setting for frequency
	static uint8_t prev_type=0;								// for locking frequency button
	
	
	if (butt > 0)											// change anything and recalculate signal only when changing params (button was pressed)
	{
		

		while ((butt & 0x0001) == 0)						// find the button position
		{
			temp++;
			butt = butt >> 1;
		}
		
		if (temp == 0 || temp == 8 ||temp==4)
		{
			if(Func==0)										// if func button is not pressed change frequency
			{
				
				if (temp == 0 && frequency < 2000)
				{
					frequency++;
				}

				if (temp == 8 && frequency > 100)
				{
					frequency--;
				}
			}
			else
			{
				if (temp == 0 && multiplier < 5)			// if F button has been pressed change the DTMF multiplier
				{
					multiplier++;
				}

				if (temp == 8 && multiplier > 1)
				{
					multiplier--;
				}
			}
		}
		else
		{
			type = temp;
		}
		
		
		if(temp==4)											// if F button pressed 
		{
			if( prev_type!=4)								// if it is not beeing pushed down
			Func^=0x01;										//flip function 0-1
		}
		else
		{
			change_size();
			
			generate_signal(frame_buffer, type, frequency, tim_set,multiplier); 
			
			update_disp(type,frequency,multiplier);
			
			send_uart(type,frequency);
		}
		
		if(type==prev_type && accum<200) //
		{
			accum++;
		}
		else
		{
			accum=0;
			prev_type=temp;
		}

	}
	else
	{
		accum=0;
		prev_type=255;
	}

}








void change_size()													// change the active frame buffer size to contain whole period
{
	static uint8_t timer_index = 0;
	uint8_t stop=0;
	arr_size = (uint16_t)(((1 / (float)frequency)) / tim_set);

	while (arr_size > 1000 && timer_index < 3)						// if the signal does not fit change the timer and try again
	{
		tim_set = timer_values[++timer_index];
		arr_size = (uint16_t)(((1 / (float)frequency)) / tim_set);
		stop=1;
	}

	while (arr_size < 255 && timer_index>0 && stop==0)				// if it fits and is small try increasing it by making the sampling frequency larger
	{
		tim_set = timer_values[--timer_index];
		arr_size = (uint16_t)(((1 / (float)frequency)) / tim_set);


	}

	switch (timer_index)// Theoretical: can switch the timer ovf period to update frequency, this however changes sampling frequency, for which will the recon. filters on output not suffice
	{
		case 0:
		TIM0_overflow_16u();
		break;

		case 1:
		TIM0_overflow_128u();
		break;
		case 2:
		TIM0_overflow_1m();
		break;
	}


}








