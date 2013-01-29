/*
 * main.c
 */

#include <msp430.h>
#include "sw_uart.h"
#include "utils.h"
#include "onewire.h"

extern unsigned char search_romcode[8]; // using global variable from onewire.c

void main(void) {
	BCSCTL1 = CALBC1_16MHZ; //set clock
	DCOCTL  = CALDCO_16MHZ;
	WDTCTL = WDTPW + WDTHOLD; //stop watchdog
	__disable_interrupt(); //we do not use any interrupts
	sw_uart_init();
	leds_init();


	unsigned char presence;
	unsigned char search_successful;
	signed char bytecount;
	unsigned char scrachpad[9];
	signed int temperature;
	unsigned int whole;
	unsigned int fract;

	for(;;){

		sw_uart_wait_for_rx();

#ifdef VERBOSE_PRINT
		led1_set_on();
		sw_uart_puts("Received start signal. Querying bus...");
		sw_uart_putnewline();
		led1_set_off();
#endif // VERBOSE_PRINT

		led2_set_on();
		presence = ow_reset();
		led2_set_off();

		if(presence){
#ifdef VERBOSE_PRINT
			led1_set_on();
			sw_uart_puts("Received presence pulse. Requesting temp. conversion");
			sw_uart_putnewline();
			led1_set_off();
#endif // VERBOSE_PRINT
			led2_set_on();
			ow_reset();
			ow_write_byte(OW_SKIPROM);
			ow_write_byte(OW_CONVERTTEMP);
			led2_set_off();
#ifdef VERBOSE_PRINT
			led1_set_on();
			sw_uart_puts("Awaiting conversion");
			sw_uart_putnewline();
			led1_set_off();
#endif // VERBOSE_PRINT
			delay_miliseconds(750);

#ifdef VERBOSE_PRINT
			led1_set_on();
			sw_uart_puts("Searching for DS18B20 devices and reading data");
			sw_uart_putnewline();
			led1_set_off();
#endif //VERBOSE_PRINT


			led2_set_on();
			ow_target_setup(OW_DS18B20_FAMILY_CODE); // Search for devices with family code 28h (DS18B20) only
			search_successful = ow_first();
			led2_set_off();
			while (search_successful)
			{
				if (search_romcode[0] != OW_DS18B20_FAMILY_CODE){ //if family code is different we stop further searching.
					break;
				}

				// Communicating with 1-wire starts
				led2_set_on();
				ow_reset();
				// selecting matching device
				ow_write_byte(OW_MATCHROM);
				for (bytecount = 0; bytecount <= 7; bytecount++){
					ow_write_byte(search_romcode[bytecount]);
				}
				// reading data from selected device
				ow_write_byte(OW_READSCRATCHPAD);
				for (bytecount = 0; bytecount <= 8; bytecount++){
					scrachpad[bytecount] = ow_read_byte();
				}
				ow_reset();

				led2_set_off();
				//1-wire communication ends

				//converting data received from device to more usable form
			    temperature = (scrachpad[1]<<8) + scrachpad[0]; // temperature is 16bit
			    whole = abs(temperature)>>4;
			    fract = (abs(temperature)&0x000F)*625;


			    //communication over uart starts
			    led1_set_on();
				#ifdef VERBOSE_PRINT
					sw_uart_puts("Scrachpad: ");
					for(bytecount = 0; bytecount <=8; bytecount++){
					sw_uart_putbyte_hex(scrachpad[bytecount]);
				}
				sw_uart_putnewline();
				sw_uart_puts("Romcode: ");
				#endif // VERBOSE_PRINT

				//printing romcode as in DS18B20 datasheet (CRC on MSB)
				for (bytecount = 7; bytecount >= 0; bytecount--){
					sw_uart_putbyte_hex(search_romcode[bytecount]);
				}

				#ifdef VERBOSE_PRINT
				sw_uart_putnewline();
				sw_uart_puts("Temperature:");
				#endif // VERBOSE_PRINT


			    sw_uart_putc(' ');
			    if(temperature<0){
				sw_uart_putc('-');
			    }else{
				sw_uart_putc('+');
				}
			    sw_uart_putint_unsigned_dec(whole,'0',3);
			    //sw_uart_putint_signed_dec(temperature>>4,1,'0',0); // whole part of temperature measurement (signed)
			    sw_uart_putc('.');
			    sw_uart_putint_unsigned_dec(fract,'0',4); // fraction part of measurement (constant length of 4 digits as the resolution is 1/16 of a degree - 0.0625 step)
				sw_uart_putnewline();
				led1_set_off();

				led2_set_on();
			    search_successful = ow_next();
			    led2_set_off();
			}
			sw_uart_putnewline(); // extra newline on end of transmission for more readable output



		}
#ifdef VERBOSE_PRINT
		else
		{
			led1_set_on();
			sw_uart_puts("Presence pulse not detected.");
			sw_uart_putnewline();
			led1_set_off();
		}
#endif //VERBOSE_PRINT

	}

}
	
