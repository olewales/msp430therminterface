/*
 * sw_uart.h
 *
 *  Created on: 03-11-2012
 *      Author: olewales
 */

#ifndef SW_UART_H_
#define SW_UART_H_

#define UART_BIT_DELAY 1000000/UART_BAUD
#define setTxHigh() do { UART_SW_TX_PORTOUT |= UART_SW_TX_PINMASK; } while(0)
#define setTxLow() do { UART_SW_TX_PORTOUT &= ~UART_SW_TX_PINMASK; } while(0)

#define isRxHigh() (UART_SW_RX_PORTIN & UART_SW_RX_PINMASK)

#define sw_uart_putnewline() do { sw_uart_putc('\r'); sw_uart_putc('\n'); } while(0)

void sw_uart_init();
void sw_uart_putc(char character); //print single character
void sw_uart_puts(char* string); //print string (null terminated array of characters)
void sw_uart_putbyte_hex(unsigned char byte); //print single byte as hex value
void sw_uart_putint_unsigned_dec(unsigned int number, unsigned char paddingchar, unsigned char minlength);
void sw_uart_putint_signed_dec(signed int number, unsigned char showplussign, unsigned char paddingchar, unsigned char minlength);

void sw_uart_wait_for_rx();

#endif /* SW_UART_H_ */
