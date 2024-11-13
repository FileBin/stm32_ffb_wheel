#include "ffb_axis.h"
#include "stm32f1xx_hal.h"
#include <stdint.h>

#define MOV_AVG_LEVEL 3
#define MOV_AVG_SIZE (1 << MOV_AVG_LEVEL)

typedef struct MovingAverage16 {
  int16_t values[MOV_AVG_SIZE];
  int32_t total;
  uint8_t index;
} MovingAverage16;

int16_t MOV_AVG_SetValue(MovingAverage16 *avg, int16_t value) {
  avg->total -= avg->values[avg->index];
  avg->total += value;
  avg->values[avg->index] = value;

  avg->index = (avg->index + 1) % MOV_AVG_SIZE;

  return (int16_t)(avg->total >> MOV_AVG_LEVEL);
}

int16_t MOV_AVG_GetValue(MovingAverage16 *avg) {}

int16_t MOV_AVG_Reset(MovingAverage16 *avg) {
  avg->index = 0;
  avg->total = 0;
}

volatile FFB_Axis FFB_axis;

MovingAverage16 position_filter;
MovingAverage16 velocity_filter;
MovingAverage16 acceleration_filter;

int16_t FFB_Axis_Update(int16_t value) {
  int32_t lastPosition = FFB_axis.position;
  value = MOV_AVG_SetValue(&position_filter, value);
  FFB_axis.position = value;

  uint32_t tmpUs = HAL_GetTick();
  int32_t td = tmpUs - FFB_axis.prevUpdateTime;
  FFB_axis.prevUpdateTime = tmpUs;

  int32_t newVelocity;
  newVelocity =
      MOV_AVG_SetValue(&velocity_filter, ((value - lastPosition) << 15) / td);

  FFB_axis.acceleration = MOV_AVG_SetValue(
      &acceleration_filter, ((newVelocity - FFB_axis.velocity) << 15) / td);
  FFB_axis.velocity = newVelocity;

  return value;
}
