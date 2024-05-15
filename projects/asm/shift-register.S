; Pin mapping:
; | ATTINY84 | SN74HC595N |
; | -------- | ---------- |
; | PINA0    | !SRCLR     |
; | PINA5    | SER        |
; | PINA2    | RCLK       |
; | PINA4    | SRCLK      |

#include <avr/io.h>

mask2       = 15        ; mask2 register
mask1       = 16        ; mask1 register
loop        = 17        ; data register
dispL       = 18        ; display low byte
dispH       = 19        ; display high byte
sleepL      = 23        ; sleep loop counter
CLRB        = PINA0
SCLK        = PINA4
RCLK        = PINA2
DATA        = PINA5

.section .text
.global main
main:
    ; set pins to output
    ldi     mask1,(1<<CLRB) | (1<<SCLK) | (1<<RCLK) | (1<<DATA)
    out     _SFR_IO_ADDR(DDRA),mask1

    ; disable CLRB (HIGH), clear others
    ldi     mask1,(1<<CLRB)
    out     _SFR_IO_ADDR(PORTA),mask1

    rcall   clear
    rcall   sleepOneSecond

wipeAll:
    ldi     dispH,0b00000000
    ldi     dispL,0b00000001
wipeAllLow:
    rcall   sendDisp
    ldi     XH,0b00000000
    ldi     XL,0b00111111
    rcall   SLEEP_millis
    cpi     dispL,0b10000000
    breq    wipeAllSetupHigh
    lsl     dispL
    rjmp    wipeAllLow

wipeAllSetupHigh:
    ldi     dispH,0b00000001
    ldi     dispL,0b00000000
wipeAllHigh:
    rcall   sendDisp
    ldi     XH,0b00000000
    ldi     XL,0b00111111
    rcall   SLEEP_millis
    cpi     dispH,0b10000000
    breq    wipeAll
    lsl     dispH
    rjmp    wipeAllHigh

clear:
    cbi     _SFR_IO_ADDR(PORTA),CLRB
    rcall   sendData
    sbi     _SFR_IO_ADDR(PORTA),CLRB
    rjmp    showData
    ret

sendDisp:
    ; we will loop over each bit in the byte
    ldi     loop,16
    ; copy the diplay value to a mask
    mov     mask2,dispH
sdBit:
    cpi     loop,8
    brne    sdComp
    mov     mask2,dispL
sdComp:
    ; check if the first bit is a 1 or 0
    ldi     mask1,0b10000000
    and     mask1,mask2
    cpi     mask1,0b10000000
    ; if not 1, send a zero
    brne    sdZero
    ; otherwise send a 1
    sbi     _SFR_IO_ADDR(PORTA),DATA
    rcall   sendData
    rjmp    sdNext
sdZero:
    cbi     _SFR_IO_ADDR(PORTA),DATA
    rcall   sendData
sdNext:
    ; shift left and send next bit
    lsl     mask2
    dec     loop
    brne    sdBit
    rjmp    showData

sendData:
    sbi     _SFR_IO_ADDR(PORTA),SCLK
    cbi     _SFR_IO_ADDR(PORTA),SCLK
    ret

showData:
    sbi     _SFR_IO_ADDR(PORTA),RCLK
    cbi     _SFR_IO_ADDR(PORTA),RCLK
    ret

sleepOneSecond:
    ldi     XH,0b00000011
    ldi     XL,0b11101000
    rjmp    SLEEP_millis

#include <1MHz/sleep.S>