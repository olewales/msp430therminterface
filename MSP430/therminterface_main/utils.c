/*
 * utils.c
 *
 *  Created on: 03-11-2012
 *      Author: olewales
 */

#include "config.h"
#include "utils.h"

#ifdef USE_LEDS // if leds support is enabled
void leds_init(){

	//LED 1 configuration
	unset_bits(LED_1_PORTSEL, LED_1_PINMASK); //GPIO
	unset_bits(LED_1_PORTSEL2, LED_1_PINMASK); //GPIO
	unset_bits(LED_1_PORTOUT, LED_1_PINMASK); //set led off (low)
	set_bits(LED_1_PORTDIR, LED_1_PINMASK); //output

	//LED 2 configuration
	unset_bits(LED_2_PORTSEL, LED_2_PINMASK); //GPIO
	unset_bits(LED_2_PORTSEL2, LED_2_PINMASK); //GPIO
	unset_bits(LED_2_PORTOUT, LED_2_PINMASK); //set led off (low)
	set_bits(LED_2_PORTDIR, LED_2_PINMASK); //output

}

void delay_miliseconds(unsigned int miliseconds){
	for(;miliseconds>0;miliseconds--){
		delay_microseconds(1000);
	}
}

#endif
