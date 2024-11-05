#include "main.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "usb_reports.h"
#include "usbd_customhid.h"
#include <stdint.h>

#define BLINK_INTERVAL 1000
#define USB_SEND_MIN_INTERVAL 10

void blink_led(void);
void process_usb(void);

void user_main(void) {
  blink_led();
  process_usb();
  HAL_Delay(50);
}

GPIO_PinState g_prev_led_state = 0;

void blink_led(void) {
  GPIO_PinState led_state =
      (HAL_GetTick() % BLINK_INTERVAL) < (BLINK_INTERVAL / 2) ? GPIO_PIN_RESET
                                                              : GPIO_PIN_SET;
  if (led_state != g_prev_led_state) {
    HAL_GPIO_WritePin(board_led_GPIO_Port, board_led_Pin, led_state);
    g_prev_led_state = led_state;
  }
}

extern USBD_HandleTypeDef hUsbDeviceFS;

uint32_t timer = 0; 

void process_usb(void) {
  USB_HID_Report report;
  USB_InitHidReport(&report);

  timer++;

  report.axes[0] = (uint16_t)((timer*100) % 0x10000);
  report.axes[1] = (uint16_t)((timer*300) % 0x10000);
  report.axes[2] = (uint16_t)((timer*500) % 0x10000);
  report.buttons = 1uL << (timer%32);

  USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, (uint8_t *)&report,
                             sizeof(USB_HID_Report));
}

void USB_InitHidReport(USB_HID_Report *hid_report) {
  hid_report->id = USB_HID_REPORT_ID;
  hid_report->axes[0] = AXIS_CENTER;
  hid_report->axes[1] = AXIS_MIN;
  hid_report->axes[2] = AXIS_MIN;
  hid_report->buttons = 0x0;
}
