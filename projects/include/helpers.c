#ifndef _ZANE_HELPERS_
#define _ZANE_HELPERS_

#define HIGH 1
#define LOW 0

#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2

#define INV_REG 0

#define sbi(var, mask) ((var) |= (uint8_t)(1 << mask))
#define cbi(var, mask) ((var) &= (uint8_t) ~(1 << mask))

#endif