#ifndef _ZANE_TN85_PINS_
#define _ZANE_TN85_PINS_

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <helpers.c>

const uint16_t pin_to_port_TBL[15] PROGMEM = {
    INV_PORT,         // 0 - INVALID
    INV_PORT,         // 1 - VCC
    (uint16_t)&PORTB, // 2
    (uint16_t)&PORTB, // 3
    (uint16_t)&PORTB, // 4
    (uint16_t)&PORTA, // 5
    (uint16_t)&PORTA, // 6
    (uint16_t)&PORTA, // 7
    (uint16_t)&PORTA, // 8
    (uint16_t)&PORTA, // 9
    (uint16_t)&PORTA, // 10
    (uint16_t)&PORTA, // 11
    (uint16_t)&PORTA, // 12
    (uint16_t)&PORTA, // 13
    INV_PORT,         // 14 - GND
};

const uint16_t pin_to_ddr_TBL[15] PROGMEM = {
    INV_PORT,        // 0 - INVALID
    INV_PORT,        // 1 - VCC
    (uint16_t)&DDRB, // 2
    (uint16_t)&DDRB, // 3
    (uint16_t)&DDRB, // 4
    (uint16_t)&DDRA, // 5
    (uint16_t)&DDRA, // 6
    (uint16_t)&DDRA, // 7
    (uint16_t)&DDRA, // 8
    (uint16_t)&DDRA, // 9
    (uint16_t)&DDRA, // 10
    (uint16_t)&DDRA, // 11
    (uint16_t)&DDRA, // 12
    (uint16_t)&DDRA, // 13
    INV_PORT,        // 14 - GND
};

const uint8_t pin_to_port_bitmask_TBL[15] PROGMEM = {
    INV_PIN, // 0 - INVALID
    INV_PIN, // 1 - VCC
    _BV(0),  // 2
    _BV(1),  // 3
    _BV(3),  // 4
    _BV(2),  // 5
    _BV(7),  // 6
    _BV(6),  // 7
    _BV(5),  // 8
    _BV(4),  // 9
    _BV(3),  // 10
    _BV(2),  // 11
    _BV(1),  // 12
    _BV(0),  // 13
    INV_PIN, // 14 - GND
};

#endif