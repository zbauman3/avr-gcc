# Super simple Makefile for getting a `.hex`.
# I need to learn `make` before doing more.

PRG           = shift-register
FREQ          = 1000000
LFUSE_BYTE    = 01100010
MCU_TARGET    = attiny84
CFLAGS        = -g -Wall -O2 -mmcu=$(MCU_TARGET) -DF_CPU=$(FREQ)

build: $(PRG).hex

build-upload: build upload

upload: $(PRG).hex
	avrdude -c usbtiny -b 19200 -p $(MCU_TARGET) -U flash:w:$(PRG).hex

setLFuse:
	avrdude -v -p $(MCU_TARGET) -c usbtiny -B8 -e -U lfuse:w:0b$(LFUSE_BYTE):m

clean:
	rm -rf $(PRG).o $(PRG).hex $(PRG).elf

$(PRG).o: $(PRG).c
	avr-gcc $(CFLAGS) -I ../include -c -o $@ $^

$(PRG).elf: $(PRG).o
	avr-gcc $(CFLAGS) -o $@ $^
	
$(PRG).hex: $(PRG).elf
	avr-objcopy -j .text -j .data -O ihex $< $@