#include "usbd_def.h"
#include <stdint.h>

uint8_t HID_GetReport(USBD_HandleTypeDef *pdev, uint16_t wValue);
uint8_t HID_SetReport(uint16_t wValue);

void HID_OutEvent(const uint8_t* pbuf, uint8_t n);