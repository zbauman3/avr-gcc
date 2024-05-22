/* Name: main.c
 * Project: Datalogger based on AVR USB driver
 * Original Author: Christian Starkjohann
 * Edited by Ryan Owens (SparkFun Electronics)
 * Creation Date: 2006-04-23
 * Edited 2009-06-30
 * Tabsize: 4
 * Copyright: (c) 2006 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: Proprietary, free under certain conditions. See Documentation.
 * This Revision: $Id: main.c 537 2008-02-28 21:13:01Z cs $
 */

#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "./usbdrv/oddebug.h"
#include "./usbdrv/usbdrv.h"

/*
Pin assignment:
PB3 = analog input (ADC3)
PB4 = analog input (ADC2) - Can alternately be used as an LED output.
PB1 = LED output

PB0, PB2 = USB data lines
*/

// #define WHITE_LED 3
// #define YELLOW_LED 1

#define UTIL_BIN4(x)                                                           \
  (uchar)((0##x & 01000) / 64 + (0##x & 0100) / 16 + (0##x & 010) / 4 +        \
          (0##x & 1))
#define UTIL_BIN8(hi, lo) (uchar)(UTIL_BIN4(hi) * 16 + UTIL_BIN4(lo))

#define sbi(var, mask) ((var) |= (uint8_t)(1 << mask))
#define cbi(var, mask) ((var) &= (uint8_t) ~(1 << mask))

/* ------------------------------------------------------------------------- */

static uchar reportBuffer[2]; /* buffer for HID reports */
static uchar idleRate;        /* in 4 ms units */
static uchar hasReport = 1;

/* ------------------------------------------------------------------------- */

const PROGMEM char
    usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] = {
        /* USB report descriptor */
        0x05, 0x01, // USAGE_PAGE (Generic Desktop)
        0x09, 0x06, // USAGE (Keyboard)
        0xa1, 0x01, // COLLECTION (Application)
        0x05, 0x07, //   USAGE_PAGE (Keyboard)
        0x19, 0xe0, //   USAGE_MINIMUM (Keyboard LeftControl)
        0x29, 0xe7, //   USAGE_MAXIMUM (Keyboard Right GUI)
        0x15, 0x00, //   LOGICAL_MINIMUM (0)
        0x25, 0x01, //   LOGICAL_MAXIMUM (1)
        0x75, 0x01, //   REPORT_SIZE (1)
        0x95, 0x08, //   REPORT_COUNT (8)
        0x81, 0x02, //   INPUT (Data,Var,Abs)
        0x95, 0x01, //   REPORT_COUNT (1)
        0x75, 0x08, //   REPORT_SIZE (8)
        0x25, 0x65, //   LOGICAL_MAXIMUM (101)
        0x19, 0x00, //   USAGE_MINIMUM (Reserved (no event indicated))
        0x29, 0x65, //   USAGE_MAXIMUM (Keyboard Application)
        0x81, 0x00, //   INPUT (Data,Ary,Abs)
        0xc0        // END_COLLECTION
};
/* We use a simplifed keyboard report descriptor which does not support the
 * boot protocol. We don't allow setting status LEDs and we only allow one
 * simultaneous key press (except modifiers). We can therefore use short
 * 2 byte input reports.
 * The report descriptor has been created with usb.org's "HID Descriptor Tool"
 * which can be downloaded from http://www.usb.org/developers/hidpage/.
 * Redundant entries (such as LOGICAL_MINIMUM and USAGE_PAGE) have been omitted
 * for the second INPUT item.
 */

/* Keyboard usage values, see usb.org's HID-usage-tables document, chapter
 * 10 Keyboard/Keypad Page for more codes.
 */
#define MOD_SHIFT_RIGHT (1 << 5)
#define KEY_A 0x04     // Keyboard a and A
#define KEY_B 0x05     // Keyboard b and B
#define KEY_C 0x06     // Keyboard c and C
#define KEY_D 0x07     // Keyboard d and D
#define KEY_E 0x08     // Keyboard e and E
#define KEY_F 0x09     // Keyboard f and F
#define KEY_G 0x0a     // Keyboard g and G
#define KEY_H 0x0b     // Keyboard h and H
#define KEY_I 0x0c     // Keyboard i and I
#define KEY_J 0x0d     // Keyboard j and J
#define KEY_K 0x0e     // Keyboard k and K
#define KEY_L 0x0f     // Keyboard l and L
#define KEY_M 0x10     // Keyboard m and M
#define KEY_N 0x11     // Keyboard n and N
#define KEY_O 0x12     // Keyboard o and O
#define KEY_P 0x13     // Keyboard p and P
#define KEY_Q 0x14     // Keyboard q and Q
#define KEY_R 0x15     // Keyboard r and R
#define KEY_S 0x16     // Keyboard s and S
#define KEY_T 0x17     // Keyboard t and T
#define KEY_U 0x18     // Keyboard u and U
#define KEY_V 0x19     // Keyboard v and V
#define KEY_W 0x1a     // Keyboard w and W
#define KEY_X 0x1b     // Keyboard x and X
#define KEY_Y 0x1c     // Keyboard y and Y
#define KEY_Z 0x1d     // Keyboard z and Z
#define KEY_1 0x1e     // Keyboard 1 and !
#define KEY_COMMA 0x36 // Keyboard , and <
#define KEY_SPACE 0x2c // Keyboard Spacebar

const char text[14] = {KEY_H, KEY_E, KEY_L, KEY_L, KEY_O, KEY_COMMA, KEY_SPACE,
                       KEY_W, KEY_O, KEY_R, KEY_L, KEY_D, KEY_1,     0};

static void buildReport(uchar isSend) {
  static uchar pressed = 0;
  static uchar last = 0;
  static uchar innitClear = 0;

  reportBuffer[0] = 0;

  if (pressed == 1 || isSend == 0) {
    reportBuffer[1] = 0;
    pressed = 0;
    return;
  }

  if (innitClear < 3) {
    reportBuffer[1] = 0;
    innitClear++;
    return;
  }

  reportBuffer[1] = text[last];

  if (reportBuffer[1] == KEY_H || reportBuffer[1] == KEY_1) {
    reportBuffer[0] = MOD_SHIFT_RIGHT;
  }

  if (reportBuffer[1] == 0) {
    hasReport = 0;
  }

  pressed = 1;
  last++;
}

/* ------------------------------------------------------------------------- */
/* ------------------------ interface to USB driver ------------------------ */
/* ------------------------------------------------------------------------- */

uchar usbFunctionSetup(uchar data[8]) {
  usbRequest_t *rq = (void *)data;

  usbMsgPtr = reportBuffer;
  if ((rq->bmRequestType & USBRQ_TYPE_MASK) ==
      USBRQ_TYPE_CLASS) { /* class request type */
    if (rq->bRequest ==
        USBRQ_HID_GET_REPORT) { /* wValue: ReportType (highbyte), ReportID
                                   (lowbyte) */
      /* we only have one report type, so don't look at wValue */
      buildReport(0);
      return sizeof(reportBuffer);
    } else if (rq->bRequest == USBRQ_HID_GET_IDLE) {
      usbMsgPtr = &idleRate;
      return 1;
    } else if (rq->bRequest == USBRQ_HID_SET_IDLE) {
      idleRate = rq->wValue.bytes[1];
    }
  } else {
    /* no vendor specific requests implemented */
  }
  return 0;
}

void usbEventResetReady(void) {}

/* ------------------------------------------------------------------------- */
/* --------------------------------- main ---------------------------------- */
/* ------------------------------------------------------------------------- */

int main(void) {
  // Initialize the USB Connection with the host computer.
  usbDeviceDisconnect();
  _delay_ms(300);
  usbDeviceConnect();

  usbInit();
  sei();
  for (;;) {
    usbPoll(); // Check to see if it's time to send a USB packet
    if (usbInterruptIsReady() && hasReport == 1) {
      buildReport(1);
      usbSetInterrupt(reportBuffer, sizeof(reportBuffer));
    }
  }
  return 0;
}
