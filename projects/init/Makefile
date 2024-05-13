# Super simple Makefile for getting a `.hex`.
# I need to learn `make` before doing more.

PRG           = main
FREQ          = 1000000
LFUSE_BYTE    = 01100010
MCU_TARGET    = attiny84
CC            = avr-gcc
CFLAGS        = -g -Wall -O2 -mmcu=$(MCU_TARGET) -DF_CPU=$(FREQ)

# dependencies:
$(PRG).o: $(PRG).c

build-upload: build upload

build: clean $(PRG).elf $(PRG).hex

$(PRG).elf: $(PRG).o
  $(CC) $(CFLAGS) -o $@ $^
  
$(PRG).hex: $(PRG).elf
  avr-objcopy -j .text -j .data -O ihex $< $@

upload:
  avrdude -c usbtiny -b 19200 -p $(MCU_TARGET) -U flash:w:$(PRG).hex

setLFuse:
  avrdude -v -p $(MCU_TARGET) -c usbtiny -B8 -e -U lfuse:w:0b$(LFUSE_BYTE):m

clean:
  rm -rf *.o $(PRG).elf *.hex