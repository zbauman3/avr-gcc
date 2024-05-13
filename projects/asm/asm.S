; for docs, see:
; redhat binutils docs: https://sourceware.org/binutils/docs-2.28/as/index.html
; avr-libc docs: https://www.nongnu.org/avr-libc/user-manual/assembler.html

#include <avr/io.h>

mask    = 16        ; mask register
ledR    = 17        ; led register
oLoopR  = 18        ; outer loop register
iLoopRl = 24        ; inner loop register low
iLoopRh = 25        ; inner loop register high
oVal    = 71        ; outer loop value
iVal    = 900       ; inner loop value

.section .text
.global main
main:
    clr     ledR                 ; clear led register
    ldi     mask,(1<<PINB0)      ; load 00000001 into mask register
    out     _SFR_IO_ADDR(DDRB),mask            ; set PINB0 to output

start:
    eor     ledR,mask            ; toggle PINB0 in led register
    out     _SFR_IO_ADDR(PORTB),ledR           ; write led register to PORTB

    ldi     oLoopR,oVal          ; initialize outer loop count

oLoop:
    ldi     iLoopRl,lo8(iVal)    ; intialize inner loop count in inner
    ldi     iLoopRh,hi8(iVal)   ; loop high and low registers

iLoop:
    sbiw    iLoopRl,1    ; decrement inner loop registers
    brne    iLoop                ; branch to iLoop if iLoop registers != 0

    dec     oLoopR               ; decrement outer loop register
    brne    oLoop                ; branch to oLoop if outer loop register != 0

    rjmp    start                ; jump back to start