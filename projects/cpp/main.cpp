#include <SNX4HC595.c>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <helpers.c>
#include <pins.c>
#include <util/delay.h>

#define ADC_PIN 12

SNX4HC595Config config = {.clrb = 13, .sclk = 9, .rclk = 11, .data = 8};

void loop() {
  SNX4HC595_sendWord(&config, 0b1100000000000000);
  _delay_ms(3000);

  uint16_t adcVal = analogRead(ADC_PIN);

  SNX4HC595_sendWord(&config, adcVal);
  _delay_ms(3000);
}

int main(void) {
  SNX4HC595_setup(&config);
  SNX4HC595_clear(&config);

  pinMode(ADC_PIN, INPUT);
  enableAdc();

  for (;;)
    loop();

  return (0);
}
