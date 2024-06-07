#ifndef _ZANE_PINS_
#define _ZANE_PINS_

#include <helpers.c>

#ifdef __AVR_ATtiny84__
#include <t84/pins.c>
#else
#error Unsupported AVR
#endif

#define pin_to_port(P) (*(pin_to_port_TBL + P))
#define pin_to_ddr(P) (*(pin_to_ddr_TBL + P))

void digitalWrite(uint8_t pin, uint8_t value) {
  uint8_t mask = pinToPortBitMask(pin);
  volatile uint8_t *port = pin_to_port(pin);

  if (port == INV_REG) {
    return;
  }

  if (value == HIGH) {
    *port |= mask;
  } else if (value == LOW) {
    *port &= ~mask;
  }
}

void pinMode(uint8_t pin, uint8_t mode) {
  uint8_t mask = pinToPortBitMask(pin);
  volatile uint8_t *port = pin_to_port(pin);
  volatile uint8_t *ddr = pin_to_ddr(pin);

  if (port == INV_REG || ddr == INV_REG) {
    return;
  }

  switch (mode) {
  case INPUT:
  case INPUT_PULLUP:
    *ddr &= ~mask;
    break;
  case OUTPUT:
    *ddr |= mask;
    break;
  }

  if (mode == INPUT) {
    *port &= ~mask;
  } else if (mode == INPUT_PULLUP) {
    *port |= mask;
  }
}

#endif