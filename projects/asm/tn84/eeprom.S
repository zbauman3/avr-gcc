; Pin mapping:
; | ATTINY84 | SN74HC595N |
; | -------- | ---------- |
; | PINA0    | !SRCLR     |
; | PINA5    | SER        |
; | PINA2    | RCLK       |
; | PINA4    | SRCLK      |

#include <avr/io.h>

shouldSave          = 19
SN74HC595N_CLRB     = PINA0
SN74HC595N_RCLK     = PINA2

.section .text
.global main
main:
    rcall   SN74HC595N_setup
    ; set PB0 as input pull-up
    cbi     _SFR_IO_ADDR(DDRB),PB0
    sbi     _SFR_IO_ADDR(PORTB),PB0
    ; enable interrupts on PB0
    sei
    ldi     r16,(1<<PCIE1)
    out     _SFR_IO_ADDR(GIMSK),r16
    ldi     r16,(1<<PCINT8)
    out     _SFR_IO_ADDR(PCMSK1),r16
    ; read EEPROM data and set the high byte value
    ldi     XH,0
    ldi     XL,1
    rcall   EEPROM_read
    mov     r17,r16
    ; initialize the `increment` counter
    ldi     r18,1

; the main loop
loop:
    rcall   saveState
    rcall   increment
    rjmp    loop

; increment the display byte
increment:
    ; if we've shifted to `0b00000000`, then reset the counter
    cpi     r18,0
    brne    incrementShow
    ldi     r18,0b00000001
incrementShow:
    rcall   showData
    ldi     XH,0b00000011
    ldi     XL,0b11101000
    rcall   SLEEP_millis
    lsl     r18
    ret

; save the selected byte
saveState:
    sbrs    shouldSave,0
    ret
    push    XH
    push    XL
    push    r16
    ldi     XH,0
    ldi     XL,1
    mov     r16,r17
    rcall   EEPROM_write
    pop     r16
    pop     XL
    pop     XH
    ret

; save the data to EEPROM
showData:
    push    r16
    mov     r16,r17
    rcall   SN74HC595N_sendByte
    mov     r16,r18
    rcall   SN74HC595N_sendByte
    rcall   SN74HC595N_show
    pop     r16
    ret

.global PCINT1_vect
; handle button press
PCINT1_vect:
    ; copy display register to selected register
    mov     r17,r18
    ; flag that the selected byte should be saved
    ldi     shouldSave,1
    reti

#include <1MHz/sleep.S>
#include <EEPROM.S>
#include <SN74HC595N.S>