
#ifndef _ZANE_HELPERS_
#define _ZANE_HELPERS_

#define HIGH 1
#define LOW 0
#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2

#define digitalWrite(port, pin, value)                                         \
  if (value == HIGH) {                                                         \
    port |= _BV(pin);                                                          \
  } else if (value == LOW) {                                                   \
    port &= ~_BV(pin);                                                         \
  }

#define digitalToggle(port, pin)                                               \
  if ((port & _BV(pin)) > 0) {                                                 \
    digitalWrite(port, pin, LOW);                                              \
  } else {                                                                     \
    digitalWrite(port, pin, HIGH);                                             \
  }

#ifdef DDRA
#define PINMODE_DDRA(ddr, pin, mode)                                           \
  if (ddr == DDRA) {                                                           \
    if (mode == INPUT) {                                                       \
      PORTA &= ~_BV(pin);                                                      \
    } else if (mode == INPUT_PULLUP) {                                         \
      PORTA |= _BV(pin);                                                       \
    }                                                                          \
  }
#else
#define PINMODE_DDRA(ddr, pin, mode)
#endif

#ifdef DDRB
#define PINMODE_DDRB(ddr, pin, mode)                                           \
  if (ddr == DDRB) {                                                           \
    if (mode == INPUT) {                                                       \
      PORTB &= ~_BV(pin);                                                      \
    } else if (mode == INPUT_PULLUP) {                                         \
      PORTB |= _BV(pin);                                                       \
    }                                                                          \
  }
#else
#define PINMODE_DDRB(ddr, pin, mode)
#endif

#ifdef DDRC
#define PINMODE_DDRC(ddr, pin, mode)                                           \
  if (ddr == DDRC) {                                                           \
    if (mode == INPUT) {                                                       \
      PORTC &= ~_BV(pin);                                                      \
    } else if (mode == INPUT_PULLUP) {                                         \
      PORTC |= _BV(pin);                                                       \
    }                                                                          \
  }
#else
#define PINMODE_DDRC(ddr, pin, mode)
#endif

#define pinMode(ddr, pin, mode)                                                \
  switch (mode) {                                                              \
  case INPUT:                                                                  \
  case INPUT_PULLUP:                                                           \
    ddr &= ~_BV(pin);                                                          \
    break;                                                                     \
  case OUTPUT:                                                                 \
    ddr |= _BV(pin);                                                           \
    break;                                                                     \
  }                                                                            \
  PINMODE_DDRA(ddr, pin, mode)                                                 \
  PINMODE_DDRB(ddr, pin, mode)                                                 \
  PINMODE_DDRC(ddr, pin, mode)

#endif