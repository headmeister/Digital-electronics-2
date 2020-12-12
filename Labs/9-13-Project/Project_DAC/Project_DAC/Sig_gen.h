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





/** @brief  Signal Buffer filling function
 *  @param  frame_buffer-pointer to frame buffer vector 
 *  @param  type- button pressed (later decoded by type_map)
 *	@param	freq- frequency of output signal in Hz should be in range 100-2000 Hz for default setting
 *	@param	tim_set - timer set ovf period
 *	@param	multiplier - extra parameter for DTMF describes the ratio of two generated sine waves
 */
void generate_signal(uint8_t* frame_buffer,uint8_t type, uint16_t freq,float tim_set,uint8_t multiplier);


/** @brief  Display updating function
 *  @param  type- button pressed
 *	@param	frequency- frequency of output signal in Hz should be in range 100-2000 Hz for default setting
 *	@param	multiplier - extra parameter for DTMF describes the ratio of two generated sine waves
 */
void update_disp(uint8_t type,uint16_t frequency,uint8_t multiplier);


/** @brief  UART send  current state function
 *  @param  type- button pressed
 *	@param	frequency- frequency of output signal in Hz should be in range 100-2000 Hz for default setting
 */
void send_uart(uint8_t type,uint16_t frequency);



/** @brief  Return generated function name function
 *  @param  text- empty char array where to store the wvf. name
 *	@param	type- button pressed
 */
void return_wvftype(char text[],uint8_t type);