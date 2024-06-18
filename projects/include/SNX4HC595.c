#ifndef _ZANE_SNX4HC595_
#define _ZANE_SNX4HC595_

#include <avr/io.h>
#include <helpers.c>
#include <pins.c>

#define USI_TGL_CLK ((1 << USIWM0) | (1 << USITC))
#define USI_TGL_CLK_SHIFT ((1 << USIWM0) | (1 << USITC) | (1 << USICLK))

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
  USIDR = value;

  // not beautiful, but efficient.
  USICR = USI_TGL_CLK;
  USICR = USI_TGL_CLK_SHIFT;
  USICR = USI_TGL_CLK;
  USICR = USI_TGL_CLK_SHIFT;
  USICR = USI_TGL_CLK;
  USICR = USI_TGL_CLK_SHIFT;
  USICR = USI_TGL_CLK;
  USICR = USI_TGL_CLK_SHIFT;
  USICR = USI_TGL_CLK;
  USICR = USI_TGL_CLK_SHIFT;
  USICR = USI_TGL_CLK;
  USICR = USI_TGL_CLK_SHIFT;
  USICR = USI_TGL_CLK;
  USICR = USI_TGL_CLK_SHIFT;
  USICR = USI_TGL_CLK;
  USICR = USI_TGL_CLK_SHIFT;

  SNX4HC595_showData(config);
}

void SNX4HC595_sendWord(SNX4HC595Config *config, uint16_t value) {
  USIDR = (value >> 8);

  // not beautiful, but efficient.
  USICR = USI_TGL_CLK;
  USICR = USI_TGL_CLK_SHIFT;
  USICR = USI_TGL_CLK;
  USICR = USI_TGL_CLK_SHIFT;
  USICR = USI_TGL_CLK;
  USICR = USI_TGL_CLK_SHIFT;
  USICR = USI_TGL_CLK;
  USICR = USI_TGL_CLK_SHIFT;
  USICR = USI_TGL_CLK;
  USICR = USI_TGL_CLK_SHIFT;
  USICR = USI_TGL_CLK;
  USICR = USI_TGL_CLK_SHIFT;
  USICR = USI_TGL_CLK;
  USICR = USI_TGL_CLK_SHIFT;
  USICR = USI_TGL_CLK;
  USICR = USI_TGL_CLK_SHIFT;

  USIDR = value;

  // not beautiful, but efficient.
  USICR = USI_TGL_CLK;
  USICR = USI_TGL_CLK_SHIFT;
  USICR = USI_TGL_CLK;
  USICR = USI_TGL_CLK_SHIFT;
  USICR = USI_TGL_CLK;
  USICR = USI_TGL_CLK_SHIFT;
  USICR = USI_TGL_CLK;
  USICR = USI_TGL_CLK_SHIFT;
  USICR = USI_TGL_CLK;
  USICR = USI_TGL_CLK_SHIFT;
  USICR = USI_TGL_CLK;
  USICR = USI_TGL_CLK_SHIFT;
  USICR = USI_TGL_CLK;
  USICR = USI_TGL_CLK_SHIFT;
  USICR = USI_TGL_CLK;
  USICR = USI_TGL_CLK_SHIFT;

  SNX4HC595_showData(config);
}

#endif