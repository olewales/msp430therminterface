/*
 * utils.h
 *
 *  Created on: 03-11-2012
 *      Author: olewales
 */

#ifndef UTILS_H_
#define UTILS_H_

#include "config.h"


#define delay_microseconds(n) do { __delay_cycles(n<<4); } while(0) // for 16MHz: 1us = 16cycles
void delay_miliseconds(unsigned int);

#define set_bits(port, bitmask) do { port |= bitmask; } while(0)
#define unset_bits(port, bitmask) do { port &= (~bitmask); } while(0)
#define toggle_bits(port, bitmask) do { port^= bitmask; } while(0)

#ifdef USE_LEDS // leds enabled

#define led1_set_on() do {set_bits(LED_1_PORTOUT, LED_1_PINMASK);} while(0)
#define led1_set_off() do {unset_bits(LED_1_PORTOUT, LED_1_PINMASK);} while(0)
#define led1_toggle() do {toggle_bits(LED_1_PORTOUT, LED_1_PINMASK);} while(0)
#define led2_set_on() do {set_bits(LED_2_PORTOUT, LED_2_PINMASK);} while(0)
#define led2_set_off() do {unset_bits(LED_2_PORTOUT, LED_2_PINMASK);} while(0)
#define led2_toggle() do {toggle_bits(LED_2_PORTOUT, LED_2_PINMASK);} while(0)
void leds_init();

#else // leds disabled - replace definitions with empty functions

#define led1_set_on() do {} while(0)
#define led1_set_off() do {} while(0)
#define led1_toggle() do {} while(0)
#define led2_set_on() do {} while(0)
#define led2_set_off() do {} while(0)
#define led2_toggle() do {} while(0)
#define leds_init() do {} while(0)

#endif // leds configuration end

#endif /* UTILS_H_ */
