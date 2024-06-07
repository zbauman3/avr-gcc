#ifndef _ZANE_T85_HELPERS_
#define _ZANE_T85_HELPERS_

#include <avr/io.h>
#include <helpers.c>

volatile uint8_t *pin_to_port_TBL[] = {
    INV_REG, &PORTB, &PORTB, &PORTB, &PORTB, &PORTA, &PORTA, &PORTA,
    &PORTA,  &PORTA, &PORTA, &PORTA, &PORTA, &PORTA, INV_REG};

volatile uint8_t *pin_to_ddr_TBL[] = {INV_REG, &DDRB, &DDRB, &DDRB, &DDRB,
                                      &DDRA,   &DDRA, &DDRA, &DDRA, &DDRA,
                                      &DDRA,   &DDRA, &DDRA, &DDRA, INV_REG};

uint8_t pinToPortBitMask(uint8_t pin) {
  if (pin >= 6) {
    return _BV(13 - pin);
  }

  switch (pin) {
  case 3:
    return _BV(1);
  case 4:
    return _BV(3);
  case 5:
    return _BV(2);
  }

  return _BV(0);
}

#endif