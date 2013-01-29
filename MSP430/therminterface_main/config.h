/*
 * config.h
 *
 *  Created on: 03-11-2012
 *      Author: olewales
 *
 *      File containing location of hardware peripherals and compile-time configuration constants
 *
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <msp430.h>

// preconfigured for TI Launchpad built-in peripherals

//RED LED - port1, pin0
#define LED_1_PORTDIR P1DIR
#define LED_1_PORTOUT P1OUT
#define LED_1_PORTSEL P1SEL
#define LED_1_PORTSEL2 P1SEL2
#define LED_1_PORTREN P1REN
#define LED_1_PORTIE P1IE
#define LED_1_PORTIES P1IES
#define LED_1_PORTIFG P1IFG
#define LED_1_PORTIN P1IN
#define LED_1_PINMASK BIT0

//GREEN LED - port1, pin6
#define LED_2_PORTDIR P1DIR
#define LED_2_PORTOUT P1OUT
#define LED_2_PORTSEL P1SEL
#define LED_2_PORTSEL2 P1SEL2
#define LED_2_PORTREN P1REN
#define LED_2_PORTIE P1IE
#define LED_2_PORTIES P1IES
#define LED_2_PORTIFG P1IFG
#define LED_2_PORTIN P1IN
#define LED_2_PINMASK BIT6

//BUTTON - port1, pin3
#define BUTTON_PORTDIR P1DIR
#define BUTTON_PORTOUT P1OUT
#define BUTTON_PORTSEL P1SEL
#define BUTTON_PORTSEL2 P1SEL2
#define BUTTON_PORTREN P1REN
#define BUTTON_PORTIE P1IE
#define BUTTON_PORTIES P1IES
#define BUTTON_PORTIFG P1IFG
#define BUTTON_PORTIN P1IN
#define BUTTON_PINMASK BIT3

//UART IN SOFTWARE CONFIGURATION (jumpers set straight)
//RECEIVE - port1, pin2
#define UART_SW_RX_PORTDIR P1DIR
#define UART_SW_RX_PORTOUT P1OUT
#define UART_SW_RX_PORTSEL P1SEL
#define UART_SW_RX_PORTSEL2 P1SEL2
#define UART_SW_RX_PORTREN P1REN
#define UART_SW_RX_PORTIE P1IE
#define UART_SW_RX_PORTIES P1IES
#define UART_SW_RX_PORTIFG P1IFG
#define UART_SW_RX_PORTIN P1IN
#define UART_SW_RX_PINMASK BIT2

//TRANSMIT - port1, pin1
#define UART_SW_TX_PORTDIR P1DIR
#define UART_SW_TX_PORTOUT P1OUT
#define UART_SW_TX_PORTSEL P1SEL
#define UART_SW_TX_PORTSEL2 P1SEL2
#define UART_SW_TX_PORTREN P1REN
#define UART_SW_TX_PORTIE P1IE
#define UART_SW_TX_PORTIES P1IES
#define UART_SW_TX_PORTIFG P1IFG
#define UART_SW_TX_PORTIN P1IN
#define UART_SW_TX_PINMASK BIT1

// ULP Advisor tip: Rule 11.2 Use lower bits for port bit-banging
// ONE WIRE BUS - port2, pin0
#define ONEWIRE_PORTDIR P2DIR
#define ONEWIRE_PORTOUT P2OUT
#define ONEWIRE_PORTSEL P2SEL
#define ONEWIRE_PORTSEL2 P2SEL2
#define ONEWIRE_PORTREN P2REN
#define ONEWIRE_PORTIE P2IE
#define ONEWIRE_PORTIES P2IES
#define ONEWIRE_PORTIFG P2IFG
#define ONEWIRE_PORTIN P2IN
#define ONEWIRE_PINMASK BIT0



//CONFIGURATION CONSTANTS
//SERIAL BAUD RATE
#define UART_BAUD 9600

//use leds?
#define USE_LEDS

//print verbose serial messages (for standalone use)
// #define VERBOSE_PRINT

#endif /* CONFIG_H_ */
