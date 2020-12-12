/************************************************************************
*  Title:    DAC signal generating library with display and uart status output
*  Author:   Jiri Vitous
*  File:     
*  Software: AVR-GCC 4.x, AVR Libc 1.4 or higher
*  Hardware: any AVR with built-in UART/USART
*  Usage:    see Doxygen manual
*
************************************************************************/

/**
 * @mainpage
 * DAC library for project in Digital Electronics 2, using Peter Fleurys libraries for LCD and UART communication.
 * @author Jiri Vitous
 * 
 *
 *  @file
 *  @defgroup Jvitous DAC Library <Sig_gen.h>
 *  @code #include <Sig_gen.h> @endcode
 *
 *  @brief	Generate signal based on entered type frequency and other parameters
 *
 * This library generates signals from LUTs or using formulas.
 * The available waveforms are: Sine, positive ramp, negative ramp, triangle, square, ECG or DTMF with parameter. 
 * The framebuffer is filled with corresponding data which can then be output to the output PORTX. The default output precision is of data type uint8_t   
 *
 *  @note Uses Peter Fleurys libraries for LCD and UART communication
 *  @author Jiri Vitous 
 */

#include <avr/io.h>			// include IO operation setting
//#include <math.h>			// Include math library for signal generation
#ifndef SIG_GEN_H_
#define SIG_GEN_H_


#endif /* SIG_GEN_H_ */


/**@{*/

/*
** function prototypes
*/
	


/** @brief  Signal Buffer filling function
 *  @param  frame_buffer Pointer to frame buffer vector 
 *  @param	type button pressed (later decoded by type_map)
 *	@param	freq frequency of output signal in Hz should be in range 100-2000 Hz for default setting
 *	@param	tim_set  Timer set ovf period, to calculate active frame buffer size
 *	@param	multiplier  Extra parameter for DTMF describes the ratio of two generated sine waves
 *  @return	none
 */
void generate_signal(uint8_t* frame_buffer,uint8_t type, uint16_t freq,float tim_set,uint8_t multiplier);


/** @brief  Display updating function
 *  @param  type Button pressed
 *	@param	frequency Frequency of output signal in Hz should be in range 100-2000 Hz for default setting
 *	@param	multiplier Extra parameter for DTMF describes the ratio of two generated sine waves
 *  @return	none
 */
void update_disp(uint8_t type,uint16_t frequency,uint8_t multiplier);


/** @brief  UART send current state function
 *  @param  type Button pressed
 *	@param	frequency Frequency of output signal in Hz should be in range 100-2000 Hz for default setting
 *  @return	none
 */
void send_uart(uint8_t type,uint16_t frequency);



/** @brief  Return generated function name function
 *  @param  text empty char array where to store the wvf. name
 *	@param	type button pressed
 *  @return	none
 */
void return_wvftype(char text[],uint8_t type);

/**@}*/