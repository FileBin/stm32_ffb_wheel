#include "config.h"

#include "usb_reports.h"
#include "usbd_def.h"
#include "usbd_ioreq.h"
#include "usbd_customhid.h"

#include "ffb.h"
#include <stdint.h>

#define HID_REPORT_TYPE_INPUT 1
#define HID_REPORT_TYPE_OUTPUT 2
#define HID_REPORT_TYPE_FEATURE 3

#include "usb_report_handler.h"

uint8_t HID_GetReport(USBD_HandleTypeDef *pdev, uint16_t wValue)
{
  uint8_t reportId = LOBYTE(wValue);
  uint8_t reportType = HIBYTE(wValue);

  if (reportType == HID_REPORT_TYPE_FEATURE)
  {
    switch (reportId)
    {
    case PID_POOL_FEATURE_REPORT_ID:
    {
      PID_PoolFeatureReport report;
      PIDPoolFeatureReport_Init(&report);
      USBD_CtlSendData(pdev, (uint8_t *)&report, sizeof(PID_PoolFeatureReport));
      return TRUE;
    }
    case PID_BLOCK_LOAD_REPORT_ID:
      uint8_t *data = (uint8_t *)FFB_GetPidBlockLoad();
      USBD_CtlSendData(pdev, data, sizeof(PID_BlockLoadReport));
      return TRUE;
    default:
      break;
    }
  }
  return FALSE;
}

uint8_t HID_SetReport(USBD_HandleTypeDef *pdev, uint16_t wValue)
{
  uint8_t reportId = LOBYTE(wValue);
  uint8_t reportType = HIBYTE(wValue);

  if (reportType == HID_REPORT_TYPE_FEATURE && reportId == CREATE_NEW_EFFECT_REPORT_ID)
  {
    PID_CreateNewEffectReport report;
    USBD_CtlPrepareRx(pdev, (uint8_t *)&report, sizeof(PID_CreateNewEffectReport));
    FFB_OnCreateNewEffect(&report);
    return TRUE;
  }
  return FALSE;
}


void HID_OutEvent(const uint8_t* pbuf, uint8_t n)
{
  FFB_OnUsbData(pbuf, n);
}
