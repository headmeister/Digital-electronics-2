/*
 * Sig_gen.c
 *
 * Created: 18.11.2020 16:43:44
 *  Author: jira
 */ 
#include <avr/io.h>			// include IO operation setting
//#include <math.h>			// Include math library for signal generation

const uint8_t lookup_sine[]=
{
	128,129,131,132,134,135,137,138,
	140,142,143,145,146,148,149,151,
	152,154,155,157,158,160,162,163,
	165,166,167,169,170,172,173,175,
	176,178,179,181,182,183,185,186,
	188,189,190,192,193,194,196,197,
	198,200,201,202,203,205,206,207,
	208,210,211,212,213,214,215,217,
	218,219,220,221,222,223,224,225,
	226,227,228,229,230,231,232,233,
	234,234,235,236,237,238,238,239,
	240,241,241,242,243,243,244,245,
	245,246,246,247,248,248,249,249,
	250,250,250,251,251,252,252,252,
	253,253,253,253,254,254,254,254,
	254,255,255,255,255,255,255,255,
	255,255,255,255,255,255,255,255,
	254,254,254,254,254,253,253,253,
	253,252,252,252,251,251,250,250,
	250,249,249,248,248,247,246,246,
	245,245,244,243,243,242,241,241,
	240,239,238,238,237,236,235,234,
	234,233,232,231,230,229,228,227,
	226,225,224,223,222,221,220,219,
	218,217,215,214,213,212,211,210,
	208,207,206,205,203,202,201,200,
	198,197,196,194,193,192,190,189,
	188,186,185,183,182,181,179,178,
	176,175,173,172,170,169,167,166,
	165,163,162,160,158,157,155,154,
	152,151,149,148,146,145,143,142,
	140,138,137,135,134,132,131,129,
	128,126,124,123,121,120,118,117,
	115,113,112,110,109,107,106,104,
	103,101,100,98,97,95,93,92,
	90,89,88,86,85,83,82,80,
	79,77,76,74,73,72,70,69,
	67,66,65,63,62,61,59,58,
	57,55,54,53,52,50,49,48,
	47,45,44,43,42,41,40,38,
	37,36,35,34,33,32,31,30,
	29,28,27,26,25,24,23,22,
	21,21,20,19,18,17,17,16,
	15,14,14,13,12,12,11,10,
	10,9,9,8,7,7,6,6,
	5,5,5,4,4,3,3,3,
	2,2,2,2,1,1,1,1,
	1,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	1,1,1,1,1,2,2,2,
	2,3,3,3,4,4,5,5,
	5,6,6,7,7,8,9,9,
	10,10,11,12,12,13,14,14,
	15,16,17,17,18,19,20,21,
	21,22,23,24,25,26,27,28,
	29,30,31,32,33,34,35,36,
	37,38,40,41,42,43,44,45,
	47,48,49,50,52,53,54,55,
	57,58,59,61,62,63,65,66,
	67,69,70,72,73,74,76,77,
	79,80,82,83,85,86,88,89,
	90,92,93,95,97,98,100,101,
	103,104,106,107,109,110,112,113,
	115,117,118,120,121,123,124,126
};

const uint8_t ecg_lookup[]=
{
	17,26,21,24,24,24,23,23,26,25,24,
	21,25,26,24,22,21,21,23,24,23,22,
	23,23,26,32,35,35,37,35,35,39,38,
	42,46,48,50,51,54,59,58,57,60,60,
	55,48,40,34,24,21,17,15,15,14,11,
	11,10,10,12,11,10,8,12,8,9,13,9,
	8,12,11,7,3,3,10,18,41,79,124,170,
	209,242,255,238,211,196,147,50,0,5,8,14,
	13,13,9,1,3,5,5,6,7,9,7,1,1,3,7,11,9,9,11,
	12,11,15,16,14,14,16,18,21,20,19,20,19,19,22,27,28,30,
	31,30,31,35,37,36,40,37,40,44,47,48,50,53,54,54,56,58,56,59,
	55,56,60,58,58,56,55,53,50,49,46,44,43,43,41,39,37,37,37,33,
	34,30,31,31,30,31,29,27,27,29,29,32,31,30,28,25,29,29,32,31,31,
	32,30,30,37,34,34,33,32,32,34,36,35,35,33,31,32,32,32,30,29,31,33,
	31,29,30,32,30,29,30,32,34,31,30,27,28,31,30,30,28,32,31,30,28,27,24
};


void generate_signal(uint8_t* frame_buffer, uint8_t type, uint16_t freq,float tim_set,uint8_t multiplier)
{
	
	
	
	
	
	uint16_t arr_length= (uint16_t)(((1/(float)freq))/tim_set);	
	uint16_t index=0;
	if(arr_length>1000)
	arr_length=1000;
	 
	
	switch (type)
	{
		case 1: // sin function
		for(uint16_t i=0; i<arr_length;i++)
		{
			
		//	*frame_buffer=(uint8_t)(127.5*cos((float)(i)*2*M_PI/(float)arr_length)+127.5);
			*frame_buffer=lookup_sine[(uint16_t)((float)i/(float)arr_length*511.0)];
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
		
		case 6: //Fullwave Rectified
		for(uint16_t i=0; i<arr_length;i++)
		{
			
			//	*frame_buffer=(uint8_t)(127.5*cos((float)(i)*2*M_PI/(float)arr_length)+127.5);
			*frame_buffer=(lookup_sine[(uint16_t)((float)i/(float)arr_length*255.0)]-128)*2;
			frame_buffer++;
		}
		break;
		
		case 7: // Halfwave Rectified
		for(uint16_t i=0; i<arr_length;i++)
		{
			
			//	*frame_buffer=(uint8_t)(127.5*cos((float)(i)*2*M_PI/(float)arr_length)+127.5);
			index=(uint16_t)((float)i/(float)arr_length*511.0);
			if(i<(arr_length/2))
			*frame_buffer=(lookup_sine[index]-128)*2;
			else
			*frame_buffer=0;
			
			frame_buffer++;
		}
		break;
		
		case 8: // Ecg
		for(uint16_t i=0; i<arr_length;i++)
		{
			
				
				//	*frame_buffer=(uint8_t)(127.5*cos((float)(i)*2*M_PI/(float)arr_length)+127.5);
				*frame_buffer=ecg_lookup[(uint16_t)((float)i/(float)arr_length*234.0)];
				frame_buffer++;
			
			//	*frame_buffer=(uint8_t)(127.5*cos((float)(i)*2*M_PI/(float)arr_length)+127.5);
			//*frame_buffer=(uint8_t)((((float)((float)lookup_sine[(uint16_t)((float)i/(float)arr_length*511.0)]-128)/128)*(float)((float)lookup_sine[index]-128)/128)*128+128);
			
			
			
			
			
		}
		break;
		
		
		case 9: // DTMF
		for(uint16_t i=0; i<arr_length;i++)
		{
			
			//	*frame_buffer=(uint8_t)(127.5*cos((float)(i)*2*M_PI/(float)arr_length)+127.5);
			
			index=(uint16_t)((float)i/(float)arr_length*511.0*multiplier);
			
			while(index>511)																	//if the index is larger than lookup table substract one period
			{
				index=index-511;
			}
			
			*frame_buffer=lookup_sine[(uint16_t)((float)i/(float)arr_length*511.0)]/2+lookup_sine[index]/2;
			frame_buffer++;
		}
		break;
		
		
		default:
		break;
	}
	
	
	
	
}

