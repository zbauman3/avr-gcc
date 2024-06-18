; for docs, see:
; redhat binutils docs: https://sourceware.org/binutils/docs-2.28/as/index.html
; avr-libc docs: https://www.nongnu.org/avr-libc/user-manual/assembler.html

#include <avr/io.h>

mask    = 16        ; mask register
ledR    = 17        ; led register

.section .text
.global main
main:
    clr     ledR                 ; clear led register
    ldi     mask,(1<<PINB0)      ; load 00000001 into mask register
    out     _SFR_IO_ADDR(DDRB),mask            ; set PINB0 to output

start:
    eor     ledR,mask            ; toggle PINB0 in led register
    out     _SFR_IO_ADDR(PORTB),ledR           ; write led register to PORTB
  
    ldi     XH,0b00000010
    ldi     XL,0b00000000
    rcall   SLEEP_millis
    rjmp    start

#include <1MHz/sleep.S>