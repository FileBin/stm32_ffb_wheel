#include <stdint.h>

typedef struct FFB_Axis {
  int16_t position;
  int16_t velocity;
  int16_t acceleration;
  uint32_t prevUpdateTime;
} FFB_Axis;

int16_t FFB_Axis_Update(int16_t new_value);
