#include <avr/io.h>

SN74HC595N_CLRB     = PINA0
SN74HC595N_RCLK     = PINA2

.global TIM0_COMPA_vect
TIM0_COMPA_vect:
    rjmp    MILLIS_interrupt

.section .text
.global main
main:
    rcall   MILLIS_setup
    rcall   SN74HC595N_setup
    rcall   SN74HC595N_clear

loopIt:
    rcall   MILLIS_get

    rcall   SN74HC595N_clear
    mov     r16,r14
    rcall   SN74HC595N_sendByte
    mov     r16,r13
    rcall   SN74HC595N_sendByte
    rcall   SN74HC595N_show
    ldi     XH,0b00000000
    ldi     XL,0b00001000
    rcall   SLEEP_millis

    rjmp    loopIt

#include <1MHz/sleep.S>
#include <1MHz/millis.S>
#include <SN74HC595N.S>
