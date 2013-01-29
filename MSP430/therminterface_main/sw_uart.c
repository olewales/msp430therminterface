/*
 * sw_uart.c
 *
 *  Created on: 03-11-2012
 *      Author: olewales
 */


#include "config.h"
#include "sw_uart.h"
#include "utils.h"

static const char hex_characters[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

void sw_uart_init(){

	//RX pin setup
	unset_bits(UART_SW_RX_PORTSEL, UART_SW_RX_PINMASK); //GPIO
	unset_bits(UART_SW_RX_PORTSEL2, UART_SW_RX_PINMASK); //GPIO
	set_bits(UART_SW_RX_PORTOUT, UART_SW_RX_PINMASK); //set resistor to pullup
	set_bits(UART_SW_RX_PORTREN, UART_SW_RX_PINMASK); //enable resistor
	unset_bits(UART_SW_RX_PORTDIR, UART_SW_RX_PINMASK); //input

	//TX pin setup
	unset_bits(UART_SW_TX_PORTSEL, UART_SW_TX_PINMASK); //GPIO
	unset_bits(UART_SW_TX_PORTSEL2, UART_SW_TX_PINMASK); //GPIO
	set_bits(UART_SW_TX_PORTOUT, UART_SW_TX_PINMASK); //set tx line idle (high)
	set_bits(UART_SW_TX_PORTDIR, UART_SW_TX_PINMASK); //output

}

void sw_uart_putc(char character){
	unsigned char bitcount;

	//start bit
	setTxLow();
	delay_microseconds(UART_BIT_DELAY);

	//character transmission
	for(bitcount=8; bitcount>0; bitcount--){

		if(character & 0x1){ // least significant bit
			setTxHigh();
		}else{
			setTxLow();
		}
		character=character>>1;
		delay_microseconds(UART_BIT_DELAY);
	}

	//stop bit
	setTxHigh();
	delay_microseconds(UART_BIT_DELAY*2); //double stop bit

}

void sw_uart_puts(char* string){
	unsigned char character_counter=0;
	while(string[character_counter] != '\0'){
		sw_uart_putc(string[character_counter]);
		character_counter++;
	}
}

void sw_uart_putbyte_hex(unsigned char byte){
	unsigned char lower, upper;
	lower = (byte & 0x0F); // 4 least significant bits (0000xxxx)
	upper = (byte >> 4); // 4 most significant bits (xxxx0000)
	sw_uart_putc(hex_characters[upper]);
	sw_uart_putc(hex_characters[lower]);
}

void sw_uart_putint_unsigned_dec(unsigned int number, unsigned char paddingchar, unsigned char minlength){
	unsigned int divider;
	unsigned char length;
	while(minlength > 5){ // Decimal representation of 16bit int is maximum 5 characters long. Padding with zeroes to desired length.
		sw_uart_putc(paddingchar);
		minlength--;
	}

	for(divider=10000, length=5; divider>1; divider=divider/10, length--){
		if(number>=divider){
			sw_uart_putc(hex_characters[number/divider]);
			number=number%divider;
		}else if (minlength>=length){
			sw_uart_putc(paddingchar);
		}
	}
	sw_uart_putc(hex_characters[number]);

}

void sw_uart_putint_signed_dec(signed int number, unsigned char showplussign, unsigned char paddingchar, unsigned char minlength){
	if(number<0){
		sw_uart_putc('-'); // if given number is negative, print -
		if(minlength!=0) minlength--;
		number=-number;
	}else{
		if(showplussign){
			sw_uart_putc('+'); // if desired, print + for positive number
			if(minlength!=0) minlength--;
		}
	}
	sw_uart_putint_unsigned_dec(number, paddingchar, minlength);
}

void sw_uart_wait_for_rx(){
	while(isRxHigh()){

	}
}
