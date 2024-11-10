#include "config.h"

#include "main.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_adc.h"
#include "stm32f1xx_hal_gpio.h"
#include "usb_reports.h"
#include "usbd_customhid.h"
#include <stdint.h>
#include "macro.h"


#define BLINK_INTERVAL 1000
#define USB_SEND_MIN_INTERVAL 10

void blink_led(void);
void process_usb(void);

void user_main(void) {
  blink_led();
  process_usb();
  HAL_Delay(5);
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

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;

extern USBD_HandleTypeDef hUsbDeviceFS;

#define ANALOG_MAX 0x0fff

ADC_ChannelConfTypeDef sConfig = {
    .Rank = ADC_REGULAR_RANK_1,
    .SamplingTime = ADC_SAMPLETIME_239CYCLES_5,
};

uint8_t channel_valid = 0;

uint32_t prev_channel_value[3];

uint8_t readAnalog(ADC_HandleTypeDef *hadc, uint32_t channel, uint32_t *out) {

  sConfig.Channel = channel;

  if (HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK) {
    return FALSE;
  }

  HAL_ADC_Start(hadc);

  if (HAL_ADC_PollForConversion(hadc, 10) != HAL_OK) {
    return FALSE;
  }
  uint32_t value = HAL_ADC_GetValue(hadc);

  if(value == prev_channel_value[channel]) {
    channel_valid |= (1 << channel);
  }

  prev_channel_value[channel] = value;

  if(channel_valid & (1 << channel)) {
    (*out) = value;
  }
  else {
    (*out) = 0; 
  }

  HAL_ADC_Stop(hadc);

  return TRUE;
}

#define STEERING_SATURATION 1.3f

#define TRUNC_AXIS(x) _MAX2(_MIN2(x, 1.f), 0.f)
#define TRUNC_AXIS_S(x) _MAX2(_MIN2(x, .5f), -.5f)

#define AXIS_TO_INT16(x) (TRUNC_AXIS(x) * 0xffff)
#define ANALOG_TO_FLOAT(x) ((float)(x & 0x0fff) / (float)ANALOG_MAX)
#define ANALOG_TO_UINT16(x) (uint16_t)((x & 0x0fff) * 0xffff / ANALOG_MAX)
#define ANALOG_TO_INT16(x) (int16_t)(ANALOG_TO_UINT16(x) + 0x8000)

char readAnalogAxes(JoystickInputReport *report) {
  HAL_ADC_Start(&hadc1);

  uint32_t analog_val;
  float axis;

  if (!readAnalog(&hadc1, ADC_CHANNEL_0, &analog_val)) {
    return FALSE;
  }

  report->steering = ANALOG_TO_INT16(analog_val);

  if (!readAnalog(&hadc2, ADC_CHANNEL_1, &analog_val)) {
    return FALSE;
  }

  axis = ANALOG_TO_FLOAT(analog_val);

  axis -= 0.1f;
  axis *= 1.f/0.9f;

  report->accelerator = (uint16_t)AXIS_TO_INT16(axis);

  if (!readAnalog(&hadc2, ADC_CHANNEL_2, &analog_val)) {
    return FALSE;
  }

    axis = ANALOG_TO_FLOAT(analog_val);

  axis -= 0.1f;
  axis *= 1.f/0.9f;

  report->brake = (uint16_t)AXIS_TO_INT16(axis);
  
  return TRUE;
}

void process_usb(void) {
  JoystickInputReport report;
  JoystickInputReport_Init(&report);

  if (!readAnalogAxes(&report)) {
    return;
  }

  USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, (uint8_t *)&report,
                             sizeof(JoystickInputReport));
}
