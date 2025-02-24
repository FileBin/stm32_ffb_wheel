#ifndef STEERING_WHEEL_DRIVER_H
#define STEERING_WHEEL_DRIVER_H

#include "stm32f1xx_hal.h"
#include <stdint.h>

typedef struct {
  uint8_t connected : 1;
  uint8_t magnet_detected : 1;
  uint8_t magnet_min_gain_overflow : 1;
  uint8_t magnet_max_gain_overflow : 1;
  int16_t revolutions;
  uint16_t wheel_angle;
  int32_t wheel_position;
} SteeringWheelDriver;

HAL_StatusTypeDef SteeringWheelDriver_Init(SteeringWheelDriver *drv);
HAL_StatusTypeDef SteeringWheelDriver_UpdateWheelPosition(SteeringWheelDriver *drv);

#endif
