#include <avr/io.h>
#include <util/delay.h>

uint8_t last = 1;

void loop() {
  if (last == 1) {
    PORTA = 0;
    last = 0;
  } else {
    PORTA = _BV(PINA6);
    last = 1;
  }

  _delay_ms(1000);
}

int main(void) {
  DDRA = _BV(PINA6);

  for (;;)
    loop();

  return (0);
}