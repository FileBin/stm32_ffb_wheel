#include <stdint.h>
#ifndef __INCLUDE_USB_REPORT__
#define __INCLUDE_USB_REPORT__

#define USB_HID_REPORT_ID 1
#define AXIS_MIN ((int16_t)0x8000)
#define AXIS_CENTER ((int16_t)0x0)
#define AXIS_MAX ((int16_t)0x7fff)

typedef struct __attribute__((packed, aligned(1))) USB_HID_Report {
  uint8_t id;
  int16_t axes[3];
  uint32_t buttons;
} USB_HID_Report;

void USB_InitHidReport(USB_HID_Report* hid_report);
#endif
