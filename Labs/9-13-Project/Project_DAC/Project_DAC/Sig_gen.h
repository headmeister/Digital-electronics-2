/*
 * Sig_gen.h
 *
 * Created: 18.11.2020 16:41:44
 *  Author: jira
 */ 
#include <avr/io.h>			// include IO operation setting
//#include <math.h>			// Include math library for signal generation
#ifndef SIG_GEN_H_
#define SIG_GEN_H_





#endif /* SIG_GEN_H_ */






void generate_signal(uint8_t* frame_buffer,uint8_t type, uint16_t freq,float tim_set,uint8_t multiplier);