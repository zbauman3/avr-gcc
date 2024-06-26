#ifndef _ZANE_PINS_
#define _ZANE_PINS_

#include <avr/pgmspace.h>
#include <helpers.c>

#ifdef __AVR_ATtiny84__
#include <tn84/pins.c>
#else
#error Unsupported AVR
#endif

#define pin_to_port(P) (volatile uint8_t *)pgm_read_word((pin_to_port_TBL + P))
#define pin_to_ddr(P) (volatile uint8_t *)pgm_read_word((pin_to_ddr_TBL + P))
#define pin_to_port_bitmask(P)                                                 \
  (uint8_t) pgm_read_byte((pin_to_port_bitmask_TBL + P))
#define pin_to_adc_mux_bitmask(P)                                              \
  (uint8_t) pgm_read_byte((pin_to_adc_mux_bitmask_TBL + P))

void digitalWrite(uint8_t pin, uint8_t value) {
  uint8_t mask = pin_to_port_bitmask(pin);
  volatile uint8_t *port = pin_to_port(pin);

  if (port == INV_PORT || mask == INV_PIN) {
    return;
  }

  if (value == HIGH) {
    *port |= mask;
  } else if (value == LOW) {
    *port &= ~mask;
  }
}

void pinMode(uint8_t pin, uint8_t mode) {
  uint8_t mask = pin_to_port_bitmask(pin);
  volatile uint8_t *port = pin_to_port(pin);
  volatile uint8_t *ddr = pin_to_ddr(pin);

  if (port == INV_PORT || ddr == INV_PORT || mask == INV_PIN) {
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

uint8_t analog_reference = 0b00;
void analogReference(uint8_t mode) {
  // can't actually set the register here because the default setting
  // will connect AVCC and the AREF pin, which would cause a short if
  // there's something connected to AREF.
  analog_reference = mode;
}

uint16_t analogRead(uint8_t pin) {
  uint8_t mask = pin_to_adc_mux_bitmask(pin);

  if (mask == INV_PIN) {
    return 0;
  }

  ADMUX = (analog_reference << 6) | mask;

  // start conversion
  sbi(ADCSRA, ADSC);

  // wait to finish
  while (bit_is_set(ADCSRA, ADSC)) {
  };

  // let the macro handle fetching values
  return ADC;
}

void enableAdc() { sbi(ADCSRA, ADEN); }
void disableAdc() { cbi(ADCSRA, ADEN); }

#endif