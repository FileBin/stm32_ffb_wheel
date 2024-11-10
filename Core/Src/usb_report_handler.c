#include "config.h"

#include "usb_reports.h"
#include "usbd_def.h"
#include "usbd_ioreq.h"
#include "usbd_customhid.h"

#include "ffb.h"

#define HID_REPORT_TYPE_INPUT 1
#define HID_REPORT_TYPE_OUTPUT 2
#define HID_REPORT_TYPE_FEATURE 3

#include "usb_report_handler.h"

uint8_t HID_GetReport(USBD_HandleTypeDef *pdev, uint16_t wValue) {
  uint8_t reportId = LOBYTE(wValue);
  uint8_t reportType = HIBYTE(wValue);

  if (reportType == HID_REPORT_TYPE_FEATURE) {
    switch (reportId) {
    case PID_POOL_FEATURE_REPORT_ID: {
      PID_PoolFeatureReport report;
      PIDPoolFeatureReport_Init(&report);
      USBD_CtlSendData(pdev, (uint8_t *)&report, sizeof(PID_PoolFeatureReport));
      return TRUE;
    }
    case PID_BLOCK_LOAD_REPORT_ID:
      const uint8_t* data = (const uint8_t*)FFB_GetPidBlockLoad();
      USBD_CtlSendData(pdev, data, sizeof(PID_BlockLoadReport));
      return TRUE;
    default:
      break;
    }
  }
  return FALSE;
}

uint8_t HID_SetReport(USBD_HandleTypeDef *pdev, uint16_t wValue) {
  uint8_t reportId = LOBYTE(wValue);
  uint8_t reportType = HIBYTE(wValue);

  if (reportType == HID_REPORT_TYPE_FEATURE
   && reportId == CREATE_NEW_EFFECT_REPORT_ID) {
    PID_CreateNewEffectReport report;
    USBD_CtlPrepareRx(pdev, &report, sizeof(PID_CreateNewEffectReport));
    FFB_OnCreateNewEffect(&report);
    return TRUE;
  }
  return FALSE;
}

extern USBD_HandleTypeDef hUsbDeviceFS;
extern volatile PIDStateReport state;
void HID_OutEvent(void) {
  PIDStateReport copy = state;
  USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, (uint8_t *)&copy,
                             sizeof(PIDStateReport));
}

