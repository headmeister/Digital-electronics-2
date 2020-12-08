/*
* Project_DAC.c
*
* Created: 18.11.2020 13:47:49
* Author : jira
*/

#include <avr/io.h>			// include IO operation setting
#include <math.h>			// Include math library for signal generation
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include "Sig_gen.h"
#include "lcd.h"
#include <stdlib.h> 
#include <string.h> 


void decode_button();
void change_size();
void update_disp(uint8_t type);
uint8_t frame_buffer[1000];	//create frame buffer for outputting waveforms

uint16_t frequency = 161;
uint16_t butt = 0;

uint16_t arr_size = 0;

float tim_set = 16e-6;

const float timer_values[] = { 16e-6,128e-6,1e-3 };

const uint8_t type_map[]={9,6,3,10,8,5,2,10,7,4,1};

int main(void)
{

	change_size();
	lcd_init(LCD_DISP_ON);
	
	update_disp(11);
	DDRD = 0xFF;											// set D as output
	DDRB = 0xFF;											// set B as output
	DDRC = 0x07;											// set necesary C pins as output

	generate_signal(frame_buffer, 1, frequency, tim_set);	// init frame buffer with sine wave of frequency 1



	TIM0_overflow_16u();
	TIM0_overflow_interrupt_enable();

	TIM2_overflow_1m();
	TIM2_overflow_interrupt_enable();


	sei();								// Enable interrupt

	/* Replace with your application code */
	while (1)
	{


	}
}


ISR(TIMER0_OVF_vect)
{
	static uint16_t index = 0;

	


	if (index < arr_size -1)
	{
		
		index++;
	}
	else
	{
		
		index=0;				// clear index 2cyc
		
		asm("STS 0x0124,R1");	// balancing branches 2cyc	
		asm("STS 0x0124,R1");	// balancing branches 2cyc
		
	}

	PORTB = frame_buffer[index];

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
		temp = PINC & 0b01111000;
		temp = temp >> 3;
		debounce_reg &= 0xFFF0;
		debounce_reg |= temp;
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

	if (ovf > 10 && debounce_reg == debounce_temp) // output debounced values if time has come and button values are stable
	{
		butt = debounce_reg;
		ovf = 0;
		decode_button();

	}




}


void decode_button(void) // decodes which button has been pressed and updates variables the button changes
{
	uint8_t temp = 0;
	static uint8_t type = 11;
	if (butt > 0)
	{

		while ((butt & 0x0001) == 0)
		{
			temp++;
			butt = butt >> 1;
		}

		if (temp == 0 || temp == 8)
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
			type = temp;
		}
		
		
		change_size();
		generate_signal(frame_buffer, type_map[type-1], frequency, tim_set); // fill the frame buffer with required data (function)
		update_disp(type);

	}

}




void change_size()													// change the active frame buffer size to contain whole period
{
	static uint8_t timer_index = 0;
	arr_size = (uint16_t)(((1 / (float)frequency)) / tim_set)-1;

	while (arr_size > 1000 && timer_index < 3)
	{
		tim_set = timer_values[++timer_index];
		arr_size = (uint16_t)(((1 / (float)frequency)) / tim_set)-1;

	}

	while (arr_size < 255 && timer_index>0)
	{
		tim_set = timer_values[--timer_index];
		arr_size = (uint16_t)(((1 / (float)frequency)) / tim_set)-1;


	}

	switch (timer_index)	// Theoretical: can switch the timer ovf period to update frequency, this however changes sampling frequency for which will the recon. filters not suffice										
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


void update_disp(uint8_t type)	//updates the LCD with the frequency
{
	lcd_gotoxy(0,0);
	lcd_puts("                ");
	lcd_gotoxy(0,0);
	lcd_puts("Gen.: ");
	char text[10];
	switch (type_map[type-1])
	{
		case 1:
		strcpy(text,"Sin");
		break;
		
		case 2:
		strcpy(text,"Ramp+");
		break;
		
		case 3:
		strcpy(text,"Ramp-");
		break;
		
		case 4:
		strcpy(text,"Square");
		break;
		
		case 5:
		strcpy(text,"Triangle");
		break;
		
		case 6:
		strcpy(text,"Ramp+");
		break;
		
		default:
		break;
	}
	
	
	lcd_puts(text);
	
	lcd_gotoxy(0,1);
	lcd_puts("                ");
	lcd_gotoxy(0,1);
	lcd_puts("Freq.: ");
	itoa(frequency,text,10);
	lcd_puts(text);
	lcd_puts(" Hz");
	
	
	
	
	
	
	
}