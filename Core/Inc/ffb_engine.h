#include "ffb.h"

uint8_t GetNextFreeEffect(void);
void AllocateEffect(uint8_t id);
void StartEffect(uint8_t id);
void StopEffect(uint8_t id);
void FreeEffect(uint8_t id);
void FreeAllEffects(void);
void FreeAllEffects(void);
void StopAllEffects(void);
int16_t FFBEngine_CalculateForce(void);
