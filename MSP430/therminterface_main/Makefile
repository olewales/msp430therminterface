#!/usr/bin/make

#CONFIGURATION
#uC type. For (new) launchpad use msp430g2553
MCU	=msp430g2553
#END OF CONFIGURATION. You should not need to edit values below


MSPDEBUG=mspdebug
DRIVER=rf2500
CC	=msp430-gcc
CFLAGS	=-Os -mmcu=$(MCU) 

all: clean build

clean: 
	rm -f *.out *.o

build: uc.out

program: uc.out
	$(MSPDEBUG) $(DRIVER) "prog uc.out"

erase:
	$(MSPDEBUG) $(DRIVER) "erase"

uc.out: main.o utils.o onewire.o sw_uart.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c
	$(CC) $(CFLAGS) -c -o $@ $^

utils.o: utils.c
	$(CC) $(CFLAGS) -c -o $@ $^

onewire.o: onewire.c
	$(CC) $(CFLAGS) -c -o $@ $^

sw_uart.o: sw_uart.c
	$(CC) $(CFLAGS) -c -o $@ $^
