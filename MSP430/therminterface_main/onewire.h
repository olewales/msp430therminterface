/*
 * onewire.h
 *
 *  Created on: 04-11-2012
 *      Author: olewales
 */

#ifndef ONEWIRE_H_
#define ONEWIRE_H_

#include "config.h" // for port onewire pin configuration
#include "utils.h" // set/unset_bits

//definitions for adapted Maxim code
#define FALSE 0
#define TRUE  1

// 1-wire master timing according to http://pdfserv.maxim-ic.com/en/an/AN126.pdf (in microseconds) in standard mode.
// delay A; min:5us recommended:6us max:15us
#define OW_A 6
// delay B; min:59us recommended:64us
#define OW_B 64
// delay C; min:60us recommended:60us max:120us
#define OW_C 60
// delay D; min:8us recommended:10us
#define OW_D 10
// delay E; min:5us recommended:9us max:12us // most time-critical delay
#define OW_E 9
// delay F; min:50us recommended:55us
#define OW_F 55
// delay G; min:0us recommended:0us// may be necessary for devices in overdrive mode. We don't need this.
#define OW_G 0
// delay H; min:480us recommended:480us max:640us
#define OW_H 500
// delay I; min:63us recommended:70us max:78us
#define OW_I 70
// delay J; min:410us recommended:410us
#define OW_J 410

// 1-Wire ROM Command Codes
#define OW_SEARCHROM   0xF0
#define OW_READROM     0x33
#define OW_MATCHROM    0x55
#define OW_SKIPROM     0xCC
#define OW_ALARMSEARCH 0xEC

// DS18B20 Function Commands
#define OW_CONVERTTEMP     0x44
#define OW_WRITESCRATCHPAD 0x4E
#define OW_READSCRATCHPAD  0xBE
#define OW_COPYSCRATCHPAD  0x48
#define OW_RECALLEE        0xB8
#define OW_READPOWERSUPPLY 0xB4

// DS18B20 family code
#define OW_DS18B20_FAMILY_CODE 0x28

// Macros for onewire bus pin state changes
#define _ow_enable_pullup() do { set_bits(ONEWIRE_PORTREN, ONEWIRE_PINMASK); } while(0)
#define _ow_disable_pullup() do { unset_bits(ONEWIRE_PORTREN, ONEWIRE_PINMASK); } while(0)
#define _ow_output_high() do { set_bits(ONEWIRE_PORTOUT, ONEWIRE_PINMASK); } while (0)
#define _ow_output_low() do { unset_bits(ONEWIRE_PORTOUT, ONEWIRE_PINMASK); } while (0)
#define _ow_direction_output() do { set_bits(ONEWIRE_PORTDIR, ONEWIRE_PINMASK); } while (0)
#define _ow_direction_input() do { unset_bits(ONEWIRE_PORTDIR, ONEWIRE_PINMASK); } while (0)

// Macros for basic bus operations
// Due to strange behavior pullup resistor is disabled each time pin goes to output state.

// Drive bus high / strong pullup - needed for parasite power
// disable pullup, set output state to high, set direction to output
#define ow_drive_high() do { _ow_disable_pullup();  _ow_output_high(); _ow_direction_output(); } while(0)
// Drive bus low
// disable pullup, set output state to low, set direction to output
#define ow_drive_low() do { _ow_disable_pullup(); _ow_output_low(); _ow_direction_output(); } while(0)

// Release bus (input with pullup)
// set direction to input, set output state to high, enable internal pullup
#define ow_release() do { _ow_direction_input(); _ow_output_high(); _ow_enable_pullup(); } while(0)

void ow_write_byte(unsigned char data);
unsigned char ow_read_byte();

// 1-wire operations according to http://pdfserv.maxim-ic.com/en/an/AN126.pdf
//
// Write 1 bit
// Send a '1' bit to the 1-Wire slaves (Write 1 time slot)
// Drive bus low, delay A, Release bus, delay B
// NOT IMPLEMENTED SEPARETELY

// Write 0 bit
// send a '0' bit to the 1-Wire slaves (Write 0 time slot)
// Drive bus low, delay C, Release bus, delay D
// NOT IMPLEMENTED SEPARETELY

// Write bit
// function writing given bit to the 1-Wire slaves
void ow_write_bit(unsigned char bit);

// Read bit
// Read a bit from the 1-Wire slaves (Read time slot)
// Drive bus low, delay A, Release bus, delay E, Sample bus to read bit from slave, Delay F
unsigned char ow_read_bit();
//
// Reset
// Reset the 1-Wire bus slave devices and ready them for a command
// Delay G, Drive bus low, delay H, Release bus, delay I, Sample bus, 0 = device(s) present, 1 = no device present, Delay J
int ow_reset();

// -----------------------------------------------------------------------------------------------------------------
// Search code adapted from Maxim reference implementation:
// http://www.maximintegrated.com/app-notes/index.mvp/id/187
// http://pdfserv.maximintegrated.com/en/an/AN187.pdf

// Find the 'first' devices on the 1-Wire bus
// Return TRUE  : device found, ROM number in search_romcode buffer
//        FALSE : no device present
int ow_first();

// Find the 'next' devices on the 1-Wire bus
// Return TRUE  : device found, ROM number in search_romcode buffer
//        FALSE : device not found, end of search
int ow_next();

// Perform the 1-Wire Search Algorithm on the 1-Wire bus using the existing
// search state.
// Return TRUE  : device found, ROM number in search_romcode buffer
//        FALSE : device not found, end of search
int ow_search();

// Verify the device with the ROM number in search_romcode buffer is present.
// Return TRUE  : device verified present
//        FALSE : device not present
int ow_verify();

// Setup the search to find the device type 'family_code' on the next call
// to ow_next() if it is present.
void ow_target_setup(unsigned char family_code);

// Setup the search to skip the current device type on the next call
// to ow_next().
void ow_family_skip_setup();
// -----------------------------------------------------------------------------------------------------------------

#endif /* ONEWIRE_H_ */
