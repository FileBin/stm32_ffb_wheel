#include "config.h"

#include "ffb_forces.h"
#include <stdint.h>
#include <stdlib.h>

int32_t sign(int32_t num) {
  if (num >= 0)
    return (+1);
  return (-1);
}

int32_t envelope(const EffectCalcData *data) {

  EnvelopeData envelope = data->effectState.envelopeData;
  int16_t magnitude = data->effectState.forceData.magnitude;
  uint16_t duration = data->effectState.duration;
  uint32_t time = data->time;

  if (time < envelope.attackTime) {
    return sign(magnitude) *
           (int32_t)(envelope.attackLevel +
                     (float)(abs(magnitude) - envelope.attackLevel) *
                         (float)time * data->attackTimeC);
  }

  if (duration == DURATION_INF)
    return magnitude;

  if (envelope.fadeTime && time > duration - envelope.fadeTime) {
    return sign(magnitude) *
           (int32_t)(envelope.fadeLevel +
                     (float)(abs(magnitude) - envelope.fadeLevel) *
                         (float)(duration - time) * data->fadeTimeC);
  }

  return magnitude;
}

int32_t FFB_ConstantForce(const EffectCalcData *data) { return envelope(data); }