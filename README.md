msp430therminterface
====================
Simple simple code for Texas Instruments MSP430 microcontrolers (written with MSPG2553 on Launchpad in mind, but should work equally well on similiar devices) which gathers data from (multiple) Dallas/Maxim DS18B20 sensors on 1-wire bus and sends in more usable form over serial interface.
Additionally provided simple python script which receives the data from serial interface and saves it to spreadsheet-friendly CSV files

Compilation and flashing:
====================
Under windows just import project to TI Code Composer Studio. Tested on version 5.2.1

Under linux you will need msp430-gcc and mspdebug (in debian-like distriubutions they can be found in packages gcc-msp430 and mspdebug)
Just connect your launchpad, enter directory "therminterface_main" and type "make all program"

