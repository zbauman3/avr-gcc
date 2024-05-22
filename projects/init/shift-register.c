#include <avr/io.h>
#include <helpers.h>
#include <util/delay.h>

#define CLRB PINA0
#define SCLK PINA4
#define RCLK PINA2
#define DATA PINA5

void sendData() {
  digitalWrite(PORTA, SCLK, HIGH);
  digitalWrite(PORTA, SCLK, LOW);
}

void showData() {
  digitalWrite(PORTA, RCLK, HIGH);
  digitalWrite(PORTA, RCLK, LOW);
}

void clear() {
  digitalWrite(PORTA, CLRB, LOW);
  sendData();
  digitalWrite(PORTA, CLRB, HIGH);
  showData();
}

void sendByte(uint8_t value) {
  uint8_t mask;
  int8_t i;

  for (i = 7; i >= 0; i--) {
    mask = value & (1 << i);
    if (mask == 0) {
      digitalWrite(PORTA, DATA, LOW);
    } else {
      digitalWrite(PORTA, DATA, HIGH);
    }
    sendData();
  }

  showData();
}

void loop() {
  static uint8_t last = 0b10101010;

  last = ~last;

  sendByte(last);
  sendByte(last);
  _delay_ms(1000);
}

int main(void) {
  pinMode(DDRA, CLRB, OUTPUT);
  pinMode(DDRA, SCLK, OUTPUT);
  pinMode(DDRA, RCLK, OUTPUT);
  pinMode(DDRA, DATA, OUTPUT);

  digitalWrite(PORTA, CLRB, HIGH);
  digitalWrite(PORTA, SCLK, LOW);
  digitalWrite(PORTA, RCLK, LOW);
  digitalWrite(PORTA, DATA, LOW);

  clear();

  for (;;)
    loop();

  return (0);
}