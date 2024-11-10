#include <stdint.h>

typedef struct EffectState {
  struct {
    uint8_t isAllocated : 1;
    uint8_t isPlaying : 1;
    uint8_t enableAxis : 1;
  };

  uint8_t direction;

  uint8_t effectType;
  uint8_t gain;

  uint16_t elapsedTime;
  uint16_t duration;

  int16_t magnitude;

  int16_t offset;
  uint16_t period;

  uint16_t attackLevel;
  uint16_t fadeLevel;
  uint16_t fadeTime;
  uint16_t attackTime;

  int16_t positiveCoefficient;
  int16_t negativeCoefficient;
  uint16_t positiveSaturation;
  uint16_t negativeSaturation;
  int16_t cpOffset;
  uint16_t deadBand;
} EffectState;

#include "usb_reports.h"

void FFB_OnCreateNewEffect(const PID_CreateNewEffectReport *report);
const PID_BlockLoadReport* FFB_GetPidBlockLoad(void);
