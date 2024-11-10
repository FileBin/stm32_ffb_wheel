#include "config.h"

#include "ffb.h"
#include <stdint.h>
#include <string.h>

#define SIZE_EFFECT sizeof(EffectState)

uint8_t nextEffectId = 1;
uint8_t devicePaused = 0;
uint8_t deviceGain = 255;

volatile EffectState gEffectStates[MAX_EFFECTS + 1] = {0};
PID_BlockLoadReport pidBlockLoad = {
    .id = PID_BLOCK_LOAD_REPORT_ID,
    .ramPoolAvailable = MEMORY_POOL_SIZE,
    .effectBlockIndex = 1,
    .blockLoadStatus = BLOCK_LOAD_SUCCESS,
};

uint8_t GetNextFreeEffect(void)
{
  if (nextEffectId >= MAX_EFFECTS)
  {
    return 0;
  }

  uint8_t id = nextEffectId;

  nextEffectId++;

  while (gEffectStates[nextEffectId].isAllocated)
  {
    if (nextEffectId >= MAX_EFFECTS)
      break; // the last spot was taken
    nextEffectId++;
  }

  gEffectStates[id].isAllocated = TRUE;
  return id;
}

void AllocateEffect(uint8_t id)
{
  memset((void *)&gEffectStates[id], 0, sizeof(EffectState));
  gEffectStates[id].isAllocated = TRUE;
}

void StartEffect(uint8_t id)
{
  if (id > MAX_EFFECTS)
    return;

  gEffectStates[id].isPlaying = TRUE;
  gEffectStates[id].elapsedTime = 0;
}

void StopEffect(uint8_t id)
{
  if (id > MAX_EFFECTS)
    return;
  gEffectStates[id].isPlaying = FALSE;
}

void FreeEffect(uint8_t id)
{
  if (id > MAX_EFFECTS)
    return;

  gEffectStates[id].isAllocated = 0;
  gEffectStates[id].isPlaying = 0;

  if (id < nextEffectId)
    nextEffectId = id;

  pidBlockLoad.ramPoolAvailable += SIZE_EFFECT;
}

void FreeAllEffects(void)
{
  nextEffectId = 1;
  memset(&gEffectStates, 0, sizeof(gEffectStates));
  pidBlockLoad.ramPoolAvailable = MEMORY_POOL_SIZE;
}

const PID_BlockLoadReport *FFB_GetPidBlockLoad(void)
{
  return &pidBlockLoad;
}

void FFB_OnCreateNewEffect(const PID_CreateNewEffectReport *report)
{
  pidBlockLoad.effectBlockIndex = GetNextFreeEffect();

  if (pidBlockLoad.effectBlockIndex == 0)
  {
    pidBlockLoad.blockLoadStatus = BLOCK_LOAD_FULL;
  }
  else
  {
    pidBlockLoad.blockLoadStatus = BLOCK_LOAD_SUCCESS;
    pidBlockLoad.ramPoolAvailable -= SIZE_EFFECT;

    volatile EffectState *effect = &gEffectStates[pidBlockLoad.effectBlockIndex];
    effect->effectType = report->effectType;
  }
}
