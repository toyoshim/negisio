#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>

#include "usbdrv.h"

PROGMEM const char usbHidReportDescriptor[] = {
  0x05, 0x01,  // usage page (desktop)
  0x09, 0x05,  // usage (gamepad)
  0xa1, 0x01,  // collection (application)
  0x15, 0x00,  // logical minimum (0)
  0x25, 0x01,  // logical maximum (1)
  0x35, 0x00,  // physical minimum (0)
  0x45, 0x01,  // physical minimum (1)
  0x75, 0x01,  // report size (1)
  0x95, 0x0f,  // report count (15)
  0x05, 0x09,  // usage page (button)
  0x19, 0x01,  // usage minimum (1)
  0x29, 0x0f,  // usage maximum (15)
  0x81, 0x02,  // input (variable)
  0x95, 0x01,  // report count (1)
  0x81, 0x01,  // input (constant)
  0x05, 0x01,  // usage page (desktop)
  0x25, 0x07,  // logical maximum (7)
  0x46, 0x3b, 0x01,  // physical maximum (315)
  0x75, 0x04,  // report size (4)
  0x95, 0x01,  // report count (1)
  0x65, 0x14,  // unit (degrees)
  0x09, 0x39,  // usage (hat switch)
  0x81, 0x42,  // input (variable, null state)
  0x65, 0x00,  // unit
  0x95, 0x01,  // report count (1)
  0x81, 0x01,  // input (constant)
  0x26, 0xff, 0x00,  // logical maximum (255)
  0x46, 0xff, 0x00,  // physical maximum (255)
  0x09, 0x30,  // usage (x)
  0x09, 0x31,  // usage (y)
  0x09, 0x32,  // usage (z)
  0x09, 0x35,  // usage (rz)
  0x75, 0x08,  // report size (8)
  0x95, 0x04,  // report count (4)
  0x81, 0x02,  // input (variable)
  0x05, 0x02,  // usage page (simulation)
  0x15, 0x00,  // logical minimum (0)
  0x26, 0xff, 0x00,  // logical maximum (255)
  0x09, 0xc5,  // usage (c5h)
  0x09, 0xc4,  // usage (c4h)
  0x09, 0x02,  // usage (02h)
  0x75, 0x08,  // report size (8)
  0x81, 0x02,  // input (array)
  0xc0,  // end collection
};
char checkUsbHidReportDescriptor[sizeof(usbHidReportDescriptor) == USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH ? 0 : -1];

uchar report[] = { 0x00, 0x00, 0x0f };

//         PD6 PB0
//    PB1          PD4 PD5
// PB3 + PB4    PD1
//    PB2      PD0

/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */

void updateReport() {
  uchar pinb = PINB;
  uchar pind = PIND;

  // _ _ _ D | C     _      B A
  report[0] =
    ((pind & 0x20) ? 0 : 0x10) |
    ((pind & 0x10) ? 0 : 0x08) |
    ((pind & 0x02) ? 0 : 0x02) |
    ((pind & 0x01) ? 0 : 0x01);

  // _ _ _ _ | START SELECT _ _
  report[1] =
    ((pind & 0x40) ? 0 : 0x08) |
    ((pinb & 0x01) ? 0 : 0x04);

  // Hat: 7 0 1
  //      6 f 2
  //      5 4 3
  switch (~pinb & 0x1e) {
    case 0x02: report[2] = 0; break;
    case 0x12: report[2] = 1; break;
    case 0x10: report[2] = 2; break;
    case 0x14: report[2] = 3; break;
    case 0x04: report[2] = 4; break;
    case 0x0c: report[2] = 5; break;
    case 0x08: report[2] = 6; break;
    case 0x0a: report[2] = 7; break;
    default: report[2] = 0x0f; break;
  }
}

usbMsgLen_t usbFunctionSetup(uchar data[8]) {
  usbRequest_t* usbReq = (usbRequest_t*)data;
  if ((usbReq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS) {
    switch (usbReq->bRequest) {
      case USBRQ_HID_GET_REPORT:
        updateReport();
        usbMsgPtr = report;
        return sizeof(report);
    }
  }
  return 0;
}

/* ------------------------------------------------------------------------- */

int __attribute__((noreturn)) main(void) {
  wdt_enable(WDTO_1S);
  PORTB |= 0x1f;  // pull-up B 4,3,2,1,0
  DDRB &= ~0x1f;  //   ... as input
  PORTD |= 0x73;  // pull-up D 6,5,4,1,0
  DDRD &= ~0x73;  //   ... as input
  usbInit();
  sei();
  for (;;) {
    wdt_reset();
    usbPoll();
    if (usbInterruptIsReady()) {
      updateReport();
      usbSetInterrupt(report, sizeof(report));
    }
  }
}

/* ------------------------------------------------------------------------- */
