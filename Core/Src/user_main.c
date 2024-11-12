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

void user_main(void)
{
  blink_led();
  process_usb();
  HAL_Delay(1);
}

GPIO_PinState g_prev_led_state = 0;

void blink_led(void)
{
  GPIO_PinState led_state =
      (HAL_GetTick() % BLINK_INTERVAL) < (BLINK_INTERVAL / 2) ? GPIO_PIN_RESET
                                                              : GPIO_PIN_SET;
  if (led_state != g_prev_led_state)
  {
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
    .SamplingTime = ADC_SAMPLETIME_55CYCLES_5,
};

uint8_t readAnalog(ADC_HandleTypeDef *hadc, uint32_t channel, uint32_t *out)
{
  sConfig.Channel = channel;

  if (HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK)
  {
    return FALSE;
  }

  HAL_ADC_Start(hadc);

  uint8_t result = (HAL_ADC_PollForConversion(hadc, 10) == HAL_OK);

  if (result)
  {
    (*out) = HAL_ADC_GetValue(hadc);
  }

  HAL_ADC_Stop(hadc);

  return result;
}

#define TRUNC_AXIS(x) _MAX2(_MIN2(x, 1.f), 0.f)
#define TRUNC_AXIS_S(x) _MAX2(_MIN2(x, .5f), -.5f)

#define AXIS_TO_INT16(x) (TRUNC_AXIS(x) * 0xffff)
#define AXIS_TO_UINT16(x) (TRUNC_AXIS(x) * 0x7fff)
#define ANALOG_TO_FLOAT(x) ((float)(x & 0x0fff) / (float)ANALOG_MAX)
#define ANALOG_TO_UINT16(x) (uint16_t)((x & 0x0fff) * 0xffff / ANALOG_MAX)
#define ANALOG_TO_INT16(x) (int16_t)(ANALOG_TO_UINT16(x) + 0x8000)

#define APPLY_DEADZONES(x, start, end) (x-(start))*(1.f/(1.f - (start + end)))

char readAnalogAxes(JoystickInputReport *report)
{
  uint32_t analog_val;
  float axis;

  if (!readAnalog(&hadc1, ADC_CHANNEL_0, &analog_val))
  {
    return FALSE;
  }

  report->steering = ANALOG_TO_INT16(analog_val);

  if (!readAnalog(&hadc2, ADC_CHANNEL_1, &analog_val))
  {
    return FALSE;
  }

  axis = ANALOG_TO_FLOAT(analog_val);

  axis = APPLY_DEADZONES(axis, .1f, .1f);

  report->accelerator = (uint16_t)AXIS_TO_UINT16(axis);

  if (!readAnalog(&hadc2, ADC_CHANNEL_2, &analog_val))
  {
    return FALSE;
  }

  axis = ANALOG_TO_FLOAT(analog_val);

  axis = APPLY_DEADZONES(axis, .1f, .1f);

  report->brake = (uint16_t)AXIS_TO_UINT16(axis);

  return TRUE;
}

void process_usb(void)
{
  JoystickInputReport report;
  JoystickInputReport_Init(&report);

  for (uint16_t i = 0;i<500;i++)
  {
    if(readAnalogAxes(&report)) {
      break;
    }
    HAL_Delay(10);
  }

  USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, (uint8_t *)&report,
                             sizeof(JoystickInputReport));
}
