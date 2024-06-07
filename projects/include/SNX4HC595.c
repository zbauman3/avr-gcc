#include <avr/io.h>
#include <helpers.c>
#include <pins.c>

typedef struct {
  uint8_t clrb;
  uint8_t sclk;
  uint8_t rclk;
  uint8_t data;
} SNX4HC595Config;

void SNX4HC595_sendData(SNX4HC595Config *config) {
  digitalWrite(config->sclk, HIGH);
  digitalWrite(config->sclk, LOW);
}

void SNX4HC595_showData(SNX4HC595Config *config) {
  digitalWrite(config->rclk, HIGH);
  digitalWrite(config->rclk, LOW);
}

void SNX4HC595_clear(SNX4HC595Config *config) {
  digitalWrite(config->clrb, LOW);
  SNX4HC595_sendData(config);
  digitalWrite(config->clrb, HIGH);
  SNX4HC595_showData(config);
}

void SNX4HC595_setup(SNX4HC595Config *config) {
  pinMode(config->clrb, OUTPUT);
  pinMode(config->sclk, OUTPUT);
  pinMode(config->rclk, OUTPUT);
  pinMode(config->data, OUTPUT);

  digitalWrite(config->clrb, HIGH);
  digitalWrite(config->sclk, LOW);
  digitalWrite(config->rclk, LOW);
  digitalWrite(config->data, LOW);

  SNX4HC595_clear(config);
}

void SNX4HC595_sendByte(SNX4HC595Config *config, uint8_t value) {
  uint8_t mask;
  int8_t i;

  for (i = 7; i >= 0; i--) {
    mask = value & (1 << i);
    if (mask == 0) {
      digitalWrite(config->data, LOW);
    } else {
      digitalWrite(config->data, HIGH);
    }
    SNX4HC595_sendData(config);
  }

  SNX4HC595_showData(config);
}

void SNX4HC595_sendWord(SNX4HC595Config *config, uint16_t value) {
  uint16_t mask;
  int8_t i;

  for (i = 15; i >= 0; i--) {
    mask = value & (1 << i);
    if (mask == 0) {
      digitalWrite(config->data, LOW);
    } else {
      digitalWrite(config->data, HIGH);
    }
    SNX4HC595_sendData(config);
  }

  SNX4HC595_showData(config);
}