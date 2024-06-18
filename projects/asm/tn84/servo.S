; for docs, see:
; redhat binutils docs: https://sourceware.org/binutils/docs-2.28/as/index.html
; avr-libc docs: https://www.nongnu.org/avr-libc/user-manual/assembler.html

#include <avr/io.h>

sOff      = 16
sOn       = 17

.section .text
.global main
main:
    clr     sOn
    ldi     sOn,(1<<PINB0)
    ldi     sOff,0
    out     _SFR_IO_ADDR(DDRB),sOn ; set PINB0 to output

start:
    out     _SFR_IO_ADDR(PORTB),sOn
  
    ldi     XH,0
    ldi     XL,1 ; change this between 1 and 2 to move the server
    rcall   SLEEP_millis

    out     _SFR_IO_ADDR(PORTB),sOff
  
    ldi     XH,0
    ldi     XL,19
    rcall   SLEEP_millis

    rjmp    start

#include <1MHz/sleep.S>