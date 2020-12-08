/*
 * Sig_gen.c
 *
 * Created: 18.11.2020 16:43:44
 *  Author: jira
 */ 
#include <avr/io.h>			// include IO operation setting
#include <math.h>			// Include math library for signal generation

void generate_signal(uint8_t* frame_buffer, uint8_t type, uint16_t freq,float tim_set)
{
	
	
	
	
	
	uint16_t arr_length= (uint16_t)(((1/(float)freq))/tim_set)-1;	
	
	
	 
	
	switch (type)
	{
		case 1: // sin function
		for(uint16_t i=0; i<arr_length;i++)
		{
			
			*frame_buffer=(uint8_t)(127.5*cos((float)(i)*2*M_PI/(float)arr_length)+127.5);
			frame_buffer++;
		}
		break;
		
		
		case 2: // positive ramp
		for(uint16_t i=0; i<arr_length;i++)
		{
			
			*frame_buffer=(uint8_t)((float)i/arr_length*255);
			frame_buffer++;
		}
		break;
		
		case 3: // negative ramp
		for(uint16_t i=0; i<arr_length;i++)
		{
			
			*frame_buffer=(uint8_t)(255-(float)i/arr_length*255);
			frame_buffer++;
		}
		break;
		
		case 4: // Square wave
		for(uint16_t i=0; i<arr_length;i++)
		{
			if(i<arr_length/2)
			*frame_buffer=0;
			else
			*frame_buffer=255;
			
			frame_buffer++;
		}
		break;
		
		case 5: // triangle
		for(uint16_t i=0; i<arr_length;i++)
		{
			if(i<arr_length/2)
			*frame_buffer=(uint8_t)((float)i/arr_length*255);
			else
			*frame_buffer=(uint8_t)(255-(float)i/arr_length*255);;
			
			frame_buffer++;
		}
		break;
		
		default:
		break;
	}
	
	
	
	
}

