#include <SNX4HC595.c>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <helpers.c>
#include <pins.c>
#include <util/delay.h>

SNX4HC595Config config = {.clrb = 13, .sclk = 9, .rclk = 11, .data = 8};

uint16_t readAdc() {
  // start conversion
  sbi(ADCSRA, ADSC);

  while (bit_is_set(ADCSRA, ADSC)) {
  };

  return ADC;
}

void loop() {
  SNX4HC595_sendWord(&config, 0b1100000000000000);
  _delay_ms(3000);

  uint16_t adcVal = readAdc();

  SNX4HC595_sendWord(&config, adcVal);
  _delay_ms(3000);
}

int main(void) {
  SNX4HC595_setup(&config);
  SNX4HC595_clear(&config);

  // ADC1, PA1
  pinMode(12, INPUT);

  // disable digital input
  DIDR0 = _BV(ADC1D);

  ADMUX = 0b000001;
  //        ^^^^^^ = MUX5..0 = ADC1

  // enable ADC
  ADCSRA = _BV(ADEN);

  for (;;)
    loop();

  return (0);
}
