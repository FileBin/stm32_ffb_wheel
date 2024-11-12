#include "config.h"

#include "ffb.h"
#include "stm32f1xx_hal.h"

#include <string.h>

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

int16_t FFBEngine_CalculateForce() {
  int32_t totalForce = 0;
  uint32_t time = HAL_GetTick();

  for (uint8_t id = 0; id <= MAX_EFFECTS; id++) {
    volatile EffectState *effect = &gEffectStates[id];
    int32_t tmpForce = 0;

    if (!effect->isAllocated || !effect->isPlaying)
      continue;

    int32_t elapsed = time - effect->startTime;
    if (effect->duration != DURATION_INF && elapsed > effect->duration) {
      StopEffect(id);
      continue;
    }

    switch (effect->effectType) {
    case ET_CONSTANT_FORCE:
    
      break;
    }
  }

  return 0;
}