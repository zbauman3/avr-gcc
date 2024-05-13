# Super simple Makefile for getting a `.hex`.
# I need to learn `make` before doing more.

PRG           = asm
MCU_TARGET    = attiny84
ASMFLAGS      = -mmcu=$(MCU_TARGET) -x assembler-with-cpp
CFLAGS        = -g -Wall -O2 -mmcu=$(MCU_TARGET)

build: $(PRG).hex

build-upload: build upload

upload: $(PRG).hex
	avrdude -c usbtiny -b 19200 -p $(MCU_TARGET) -U flash:w:$(PRG).hex

clean:
	rm -rf $(PRG).o $(PRG).hex $(PRG).elf

$(PRG).o: $(PRG).S
	avr-gcc $(ASMFLAGS) -c -o $@ $^

$(PRG).elf: $(PRG).o
	avr-gcc $(CFLAGS) -o $@ $^
	
$(PRG).hex: $(PRG).elf
	avr-objcopy -R .eeprom -O ihex $< $@