#include "config.h"
#include "util.h"

#include "input.h"

#include "ffb_axis.h"
#include "usb_reports.h"

#include "usbd_def.h"
#include "usbd_customhid.h"

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;

extern USBD_HandleTypeDef hUsbDeviceFS;

#define ANALOG_MAX 0x0fff

ADC_ChannelConfTypeDef sConfig = {
    .Rank = ADC_REGULAR_RANK_1,
    .SamplingTime = ADC_SAMPLETIME_55CYCLES_5,
};

uint8_t readAnalog(ADC_HandleTypeDef *hadc, uint32_t channel, uint32_t *out) {
  sConfig.Channel = channel;

  if (HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK) {
    return FALSE;
  }

  HAL_ADC_Start(hadc);

  uint8_t result = (HAL_ADC_PollForConversion(hadc, 10) == HAL_OK);

  if (result) {
    (*out) = HAL_ADC_GetValue(hadc);
  }

  HAL_ADC_Stop(hadc);

  return result;
}

#define TRUNC_AXIS(x) _CONSTRAIN(x, 0.f, 1.f)
#define TRUNC_AXIS_S(x) _CONSTRAIN(x, -.5f, .5f)

#define AXIS_TO_INT16(x) (TRUNC_AXIS(x) * 0xffff)
#define AXIS_TO_UINT16(x) (TRUNC_AXIS(x) * 0x7fff)
#define ANALOG_TO_FLOAT(x) ((float)(x & 0x0fff) / (float)ANALOG_MAX)
#define ANALOG_TO_UINT16(x) (uint16_t)((x & 0x0fff) * 0xffff / ANALOG_MAX)
#define ANALOG_TO_INT16(x) (int16_t)(ANALOG_TO_UINT16(x) + 0x8000)

#define APPLY_DEADZONES(x, start, end)                                         \
  (x - (start)) * (1.f / (1.f - (start + end)))

char readAnalogAxes(JoystickInputReport *report) {
  uint32_t analog_val;
  report->buttons = 0;
  float axis;

  if (!readAnalog(&hadc1, ADC_CHANNEL_0, &analog_val)) {
    return FALSE;
  }
  int32_t tmp = ANALOG_TO_INT16(analog_val);
  tmp *= 3;
  tmp /= 2;
  tmp = constrain(tmp, -32768, 32767);
  report->steering = FFB_Axis_Update((int16_t)tmp);

  if (!readAnalog(&hadc2, ADC_CHANNEL_1, &analog_val)) {
    return FALSE;
  }

  axis = ANALOG_TO_FLOAT(analog_val);

  axis = APPLY_DEADZONES(axis, .1f, .1f);

  report->accelerator = (uint16_t)AXIS_TO_UINT16(axis);

  if (!readAnalog(&hadc2, ADC_CHANNEL_2, &analog_val)) {
    return FALSE;
  }

  axis = ANALOG_TO_FLOAT(analog_val);

  axis = APPLY_DEADZONES(axis, .1f, .1f);

  report->brake = (uint16_t)AXIS_TO_UINT16(axis);

  if (!readAnalog(&hadc2, ADC_CHANNEL_3, &analog_val)) {
    return FALSE;
  }

  axis = ANALOG_TO_FLOAT(analog_val);

  if (axis > .75f) {
    report->buttons |= 1 << 4;
  }

  if (axis < .25f) {
    report->buttons |= 1 << 5;
  }

  return TRUE;
}

void sendReport(JoystickInputReport *report);

void process_input(void) {
  JoystickInputReport report;
  JoystickInputReport_Init(&report);

  for (uint16_t i = 0; i < 500; i++) {
    if (readAnalogAxes(&report)) {
      break;
    }
    HAL_Delay(10);
  }

  sendReport(&report);
}

uint32_t USB_lastTime = 0;
#define USB_FREQ_MS 10

void sendReport(JoystickInputReport *report) {
  uint32_t prevTime = USB_lastTime;
  uint32_t time = HAL_GetTick();

  if (time < prevTime) {
    USB_lastTime = time;
    return;
  }

  if (time < prevTime + USB_FREQ_MS) {
    return;
  }

  USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, (uint8_t *)report,
                             sizeof(JoystickInputReport));
}
