/*
 * onewire.c
 *
 *  Created on: 04-11-2012
 *      Author: olewales
 */

#include "onewire.h"
#include "sw_uart.h"

// global search state
//unsigned char search_romcode[8];
int search_last_discrepancy;
int search_last_family_discrepancy;
int search_last_device_flag;
unsigned char search_crc8;
unsigned char search_romcode[8];




//--------------------------------------------------------------------------
// Calculate the CRC8 of the byte value provided with the current
// global 'crc8' value.
// Returns current global crc8 value
//
unsigned char docrc8(unsigned char value)
{
	// crc lookup table
	static const unsigned char dscrc_table[] = {
	        0, 94,188,226, 97, 63,221,131,194,156,126, 32,163,253, 31, 65,
	      157,195, 33,127,252,162, 64, 30, 95,  1,227,189, 62, 96,130,220,
	       35,125,159,193, 66, 28,254,160,225,191, 93,  3,128,222, 60, 98,
	      190,224,  2, 92,223,129, 99, 61,124, 34,192,158, 29, 67,161,255,
	       70, 24,250,164, 39,121,155,197,132,218, 56,102,229,187, 89,  7,
	      219,133,103, 57,186,228,  6, 88, 25, 71,165,251,120, 38,196,154,
	      101, 59,217,135,  4, 90,184,230,167,249, 27, 69,198,152,122, 36,
	      248,166, 68, 26,153,199, 37,123, 58,100,134,216, 91,  5,231,185,
	      140,210, 48,110,237,179, 81, 15, 78, 16,242,172, 47,113,147,205,
	       17, 79,173,243,112, 46,204,146,211,141,111, 49,178,236, 14, 80,
	      175,241, 19, 77,206,144,114, 44,109, 51,209,143, 12, 82,176,238,
	       50,108,142,208, 83, 13,239,177,240,174, 76, 18,145,207, 45,115,
	      202,148,118, 40,171,245, 23, 73,  8, 86,180,234,105, 55,213,139,
	       87,  9,235,181, 54,104,138,212,149,203, 41,119,244,170, 72, 22,
	      233,183, 85, 11,136,214, 52,106, 43,117,151,201, 74, 20,246,168,
	      116, 42,200,150, 21, 75,169,247,182,232, 10, 84,215,137,107, 53};

	search_crc8 = dscrc_table[search_crc8 ^ value];
	return search_crc8;
}


// Search code adapted from Maxim reference implementation:
// http://www.maximintegrated.com/app-notes/index.mvp/id/187
// http://pdfserv.maximintegrated.com/en/an/AN187.pdf

 //--------------------------------------------------------------------------
 // Find the 'first' devices on the 1-Wire bus
 // Return TRUE  : device found, ROM number in search_romcode buffer
 //        FALSE : no device present
 //
 int ow_first()
 {
    // reset the search state
    search_last_discrepancy = 0;
    search_last_device_flag = FALSE;
    search_last_family_discrepancy = 0;

    return ow_search();
 }

 //--------------------------------------------------------------------------
 // Find the 'next' devices on the 1-Wire bus
 // Return TRUE  : device found, ROM number in search_romcode buffer
 //        FALSE : device not found, end of search
 //
 int ow_next()
 {
    // leave the search state alone
    return ow_search();
 }

 //--------------------------------------------------------------------------
 // Perform the 1-Wire Search Algorithm on the 1-Wire bus using the existing
 // search state.
 // Return TRUE  : device found, ROM number in search_romcode buffer
 //        FALSE : device not found, end of search
 //

 int ow_search()
 {
    int id_bit_number;
    int last_zero, rom_byte_number, search_result;
    int id_bit, cmp_id_bit;
    unsigned char rom_byte_mask, search_direction;

    // initialize for search
    id_bit_number = 1;
    last_zero = 0;
    rom_byte_number = 0;
    rom_byte_mask = 1;
    search_result = 0;
    search_crc8 = 0;

    // if the last call was not the last one
    if (!search_last_device_flag)
    {
       // 1-Wire reset
       if (!ow_reset())
       {
          // reset the search
          search_last_discrepancy = 0;
          search_last_device_flag = FALSE;
          search_last_family_discrepancy = 0;
          return FALSE;
       }

       // issue the search command
       ow_write_byte(0xF0);

       // loop to do the search
       do
       {
          // read a bit and its complement
          id_bit = ow_read_bit();
          cmp_id_bit = ow_read_bit();

          // check for no devices on 1-wire
          if ((id_bit == 1) && (cmp_id_bit == 1))
             break;
          else
          {
             // all devices coupled have 0 or 1
             if (id_bit != cmp_id_bit)
                search_direction = id_bit;  // bit write value for search
             else
             {
                // if this discrepancy if before the Last Discrepancy
                // on a previous next then pick the same as last time
                if (id_bit_number < search_last_discrepancy)
                   search_direction = ((search_romcode[rom_byte_number] & rom_byte_mask) > 0);
                else
                   // if equal to last pick 1, if not then pick 0
                   search_direction = (id_bit_number == search_last_discrepancy);

                // if 0 was picked then record its position in LastZero
                if (search_direction == 0)
                {
                   last_zero = id_bit_number;

                   // check for Last discrepancy in family
                   if (last_zero < 9)
                      search_last_family_discrepancy = last_zero;
                }
             }

             // set or clear the bit in the ROM byte rom_byte_number
             // with mask rom_byte_mask
             if (search_direction == 1)
               search_romcode[rom_byte_number] |= rom_byte_mask;
             else
               search_romcode[rom_byte_number] &= ~rom_byte_mask;

             // serial number search direction write bit
             ow_write_bit(search_direction);

             // increment the byte counter id_bit_number
             // and shift the mask rom_byte_mask
             id_bit_number++;
             rom_byte_mask <<= 1;

             // if the mask is 0 then go to new SerialNum byte rom_byte_number and reset mask
             if (rom_byte_mask == 0)
             {
                 docrc8(search_romcode[rom_byte_number]);  // accumulate the CRC
                 rom_byte_number++;
                 rom_byte_mask = 1;
             }
          }
       }
       while(rom_byte_number < 8);  // loop until through all ROM bytes 0-7

       // if the search was successful then
       if (!((id_bit_number < 65) || (search_crc8 != 0)))
       {
          // search successful so set LastDiscrepancy,search_last_device_flag,search_result
          search_last_discrepancy = last_zero;

          // check for last device
          if (search_last_discrepancy == 0)
             search_last_device_flag = TRUE;

          search_result = TRUE;
       }
    }

    // if no device found then reset counters so next 'search' will be like a first
    if (!search_result || !search_romcode[0])
    {
       search_last_discrepancy = 0;
       search_last_device_flag = FALSE;
       search_last_family_discrepancy = 0;
       search_result = FALSE;
    }

    return search_result;
 }

 //--------------------------------------------------------------------------
 // Verify the device with the ROM number in search_romcode buffer is present.
 // Return TRUE  : device verified present
 //        FALSE : device not present
 //
 int ow_verify()
 {
    unsigned char rom_backup[8];
    int i,rslt,ld_backup,ldf_backup,lfd_backup;

    // keep a backup copy of the current state
    for (i = 0; i < 8; i++)
       rom_backup[i] = search_romcode[i];
    ld_backup = search_last_discrepancy;
    ldf_backup = search_last_device_flag;
    lfd_backup = search_last_family_discrepancy;

    // set search to find the same device
    search_last_discrepancy = 64;
    search_last_device_flag = FALSE;

    if (ow_search())
    {
       // check if same device found
       rslt = TRUE;
       for (i = 0; i < 8; i++)
       {
          if (rom_backup[i] != search_romcode[i])
          {
             rslt = FALSE;
             break;
          }
       }
    }
    else
      rslt = FALSE;

    // restore the search state
    for (i = 0; i < 8; i++)
       search_romcode[i] = rom_backup[i];
    search_last_discrepancy = ld_backup;
    search_last_device_flag = ldf_backup;
    search_last_family_discrepancy = lfd_backup;

    // return the result of the verify
    return rslt;
 }

 //--------------------------------------------------------------------------
 // Setup the search to find the device type 'family_code' on the next call
 // to OWNext() if it is present.
 //
 void ow_target_setup(unsigned char family_code)
 {
    int i;

    // set the search state to find SearchFamily type devices
    search_romcode[0] = family_code;
    for (i = 1; i < 8; i++)
    	 search_romcode[i] = 0;
    search_last_discrepancy = 64;
    search_last_family_discrepancy = 0;
    search_last_device_flag = FALSE;
 }

 //--------------------------------------------------------------------------
 // Setup the search to skip the current device type on the next call
 // to OWNext().
 //
 void ow_family_skip_setup()
 {
    // set the Last discrepancy to last family discrepancy
    search_last_discrepancy = search_last_family_discrepancy;
    search_last_family_discrepancy = 0;

    // check for end of list
    if (search_last_discrepancy == 0)
       search_last_device_flag = TRUE;
 }





int ow_reset()
{
  int presence;

  delay_microseconds( OW_G );
  ow_drive_low();
  delay_microseconds( OW_H );
  ow_release();
  delay_microseconds( OW_I );

  if(ONEWIRE_PORTIN & ONEWIRE_PINMASK )
  {
    presence = 0;     // bus is high, slave did not pull it down
  }
  else
  {
    presence = 1;      // bus is being pulled down, presence detected
  }

  delay_microseconds( OW_J );
  return ( presence );        // return the sampled presence pulse result
}


void ow_write_byte(unsigned char data)
{
  unsigned char bitcount;
  for(bitcount=8; bitcount>0; bitcount--)
  {
    if (data & 0x01)
    {
    	ow_drive_low();
    	delay_microseconds(OW_A);
    	ow_release();
    	delay_microseconds(OW_B);
    }
    else
    {
    	ow_drive_low();
    	delay_microseconds(OW_C);
    	ow_release();
    	delay_microseconds(OW_D);
    }
    data = data >> 1;
  }
}

unsigned char ow_read_byte()
{
  unsigned char data=0;
  unsigned char bitcount, bit;
  for ( bitcount=8; bitcount>0; bitcount-- )
  {
	ow_drive_low();
    delay_microseconds(OW_A);
	ow_release();
    delay_microseconds(OW_E);
    if (ONEWIRE_PORTIN & ONEWIRE_PINMASK){
    	bit = 1;
    }
    else
    {
    	bit = 0;
    }
    delay_microseconds(OW_F);
    //on 1-wire bus data is sent starting with least significant bit. We need to inert bits from the left and shift.
    data = data >> 1;
    data |= (bit << 7);
  }
  return(data);
}


unsigned char ow_read_bit()
{
	unsigned char bit;

	ow_drive_low();
	delay_microseconds(OW_A);
	ow_release();
    delay_microseconds(OW_E);
    if (ONEWIRE_PORTIN & ONEWIRE_PINMASK){
    	bit = 1;
    }
    else
    {
    	bit = 0;
    }
    delay_microseconds(OW_F);
  	return(bit);
}

void ow_write_bit(unsigned char bit)
{
    if (!bit)
    {
    	ow_drive_low();
    	delay_microseconds(OW_C);
    	ow_release();
    	delay_microseconds(OW_D);
    }
    else
    {
    	ow_drive_low();
    	delay_microseconds(OW_A);
    	ow_release();
    	delay_microseconds(OW_B);
    }
}
