#include "config.h"

#include "ffb_engine.h"
#include "ffb_forces.h"
#include "stm32f1xx_hal.h"

#include <stdint.h>
#include <string.h>

#include "util.h"


#define SIZE_EFFECT sizeof(EffectState)

uint8_t nextEffectId = 1;
volatile uint8_t g_deviceGain = 255;

volatile PIDStateReport g_state = {
    .id = PID_STATE_REPORT_ID,
    .devicePaused = 0,
    .actuatorsEnabled = 1,
    .actuatorPower = 1,
    .actuatorOverrideSwitch = 1,
    .effectPlaying = 0,
    .effectBlockIndex = 0,
};

volatile PID_BlockLoadReport pidBlockLoad = {
    .id = PID_BLOCK_LOAD_REPORT_ID,
    .ramPoolAvailable = MEMORY_POOL_SIZE,
    .effectBlockIndex = 1,
    .blockLoadStatus = BLOCK_LOAD_SUCCESS,
};

volatile EffectState gEffectStates[MAX_EFFECTS + 1] = {0};

uint8_t GetNextFreeEffect(void) {
  if (nextEffectId >= MAX_EFFECTS) {
    return 0;
  }

  uint8_t id = nextEffectId;

  nextEffectId++;

  while (gEffectStates[nextEffectId].isAllocated) {
    if (nextEffectId >= MAX_EFFECTS)
      break; // the last spot was taken
    nextEffectId++;
  }

  gEffectStates[id].isAllocated = TRUE;
  return id;
}

void AllocateEffect(uint8_t id) {
  memset((void *)&gEffectStates[id], 0, sizeof(EffectState));
  gEffectStates[id].isAllocated = TRUE;
}

void StartEffect(uint8_t id) {
  if (id > MAX_EFFECTS)
    return;

  gEffectStates[id].isPlaying = TRUE;
  gEffectStates[id].startTime = HAL_GetTick();
}

void StopEffect(uint8_t id) {
  if (id > MAX_EFFECTS)
    return;
  gEffectStates[id].isPlaying = FALSE;
}

void FreeEffect(uint8_t id) {
  if (id > MAX_EFFECTS)
    return;

  gEffectStates[id].isAllocated = 0;
  gEffectStates[id].isPlaying = 0;

  if (id < nextEffectId)
    nextEffectId = id;

  pidBlockLoad.ramPoolAvailable += SIZE_EFFECT;
}

void FreeAllEffects(void) {
  nextEffectId = 1;
  memset((void *)&gEffectStates, 0, sizeof(gEffectStates));
  pidBlockLoad.ramPoolAvailable = MEMORY_POOL_SIZE;
}

void StopAllEffects(void) {
  for (uint8_t id = 0; id <= MAX_EFFECTS; id++)
    StopEffect(id);
}

void InitCalcData(EffectCalcData *data) {
  switch (data->effect.effectType) {
  case ET_RAMP:
    data->periodC = 2.0f / data->effect.duration;
    break;
  case ET_SINE:
    data->periodC = 65535.0f / data->effect.forceData.periodic.period;
    break;
  case ET_TRIANGLE:
    data->periodC = 4.0f / data->effect.forceData.periodic.period;
    break;
  case ET_SAWTOOTH_UP:
  case ET_SAWTOOTH_DOWN:
    data->periodC = 2.0f / data->effect.forceData.periodic.period;
    break;
  default:
    break;
  }

  EnvelopeData *envelope = &data->effect.envelopeData;

  if (envelope->fadeTime || envelope->attackTime) {
    if (envelope->fadeTime > data->effect.duration - envelope->attackTime)
      envelope->fadeTime = data->effect.duration - envelope->attackTime;

    data->fadeTimeC = 1.f / envelope->fadeTime;
    data->attackTimeC = 1.f / envelope->attackTime;
  }
}

int16_t FFBEngine_CalculateForce(void) {
  int32_t totalForce = 0;
  uint32_t time = HAL_GetTick();

  for (uint8_t id = 0; id <= MAX_EFFECTS; id++) {

    totalForce += FFBEngine_CalculateEffectForce(&gEffectStates[id], time);
  }

  return (int16_t)constrain(totalForce, -16383, 16383);
}

int32_t FFBEngine_CalculateEffectForce(volatile const EffectState *effectState, uint32_t time) {
  EffectCalcData data = {
      .effect = *effectState,
  };
  int32_t tmpForce = 0;

  if (!data.effect.isAllocated || !data.effect.isPlaying)
    return 0;

  data.elapsed = time - data.effect.startTime;

  if (data.effect.duration != DURATION_INF &&
      data.elapsed > data.effect.duration) {
    data.effect.isPlaying = FALSE;
    return 0;
  }

  InitCalcData(&data);

  switch (data.effect.effectType) {
  case ET_CONSTANT_FORCE:
    tmpForce = FFB_ConstantForce(&data);
    break;
  case ET_SQUARE:
  case ET_SINE:
  case ET_TRIANGLE:
  case ET_SAWTOOTH_DOWN:
  case ET_SAWTOOTH_UP:
    data.periodTime = data.elapsed % data.effect.forceData.periodic.period;
    tmpForce = FFB_PeriodicForce(&data);
    break;
  case ET_RAMP:
    tmpForce = FFB_RampForce(&data);
    break;
  case ET_SPRING:
    tmpForce = FFB_SpringForce(&data);
    break;
  case ET_DAMPER:
    tmpForce = FFB_DamperForce(&data);
    break;
  case ET_INERTIA:
    tmpForce = FFB_InertiaForce(&data);
    break;
  case ET_FRICTION:
    tmpForce = FFB_FrictionForce(&data);
    break;
  default:
    break;
  }

  tmpForce = (tmpForce * (data.effect.gain + 1)) >> 8;

  return constrain(tmpForce, -16383, 16383);
}
