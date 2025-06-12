#include "steering_wheel_driver.h"
#include "as5600_ll.h"
#include "stm32f103xb.h"
#include "stm32f1xx_hal_def.h"
#include "stm32f1xx_hal_i2c.h"
#include <stdint.h>
#include <string.h>

#define REVOLUTION_THRESHOLD 500
#define REVOLUTION_MAX 4096

#define REVOLUTION_PREV_MIN (REVOLUTION_MAX - REVOLUTION_THRESHOLD)
#define REVOLUTION_NEXT_MAX REVOLUTION_THRESHOLD

HAL_StatusTypeDef UpdateMagnetStatus(SteeringWheelDriver *drv) {
  HAL_StatusTypeDef status = HAL_OK;

  uint8_t magnet_status;
  status = AS5600_LL_GetMagnetStatus(&magnet_status);

  if (status == HAL_OK) {
    drv->magnet_detected = (magnet_status & AS5600_MAGNET_DETECTED) != 0;
    drv->magnet_min_gain_overflow =
        (magnet_status & AS5600_AGC_MIN_GAIN_OVERFLOW) != 0;
    drv->magnet_max_gain_overflow =
        (magnet_status & AS5600_AGC_MAX_GAIN_OVERFLOW) != 0;
    drv->connected = 1;
  } else {
    drv->connected = 0;
  }
  return status;
}

HAL_StatusTypeDef ResetSteeringWheelDriver(void) {
  AS5600_InitStruct init_struct = {
      .PositiveRotationDirection = AS5600_DIR_CW,
      .SlowFilter = AS5600_SLOW_FILTER_2X,
  };

  return AS5600_LL_Init(&init_struct);
}

HAL_StatusTypeDef SteeringWheelDriver_Init(SteeringWheelDriver *drv) {
  HAL_StatusTypeDef status;

  memset(drv, 0, sizeof(SteeringWheelDriver));

  status = ResetSteeringWheelDriver();
  if (status != HAL_OK)
    return status;

  status = UpdateMagnetStatus(drv);
  if (status != HAL_OK)
    return status;

  if (drv->magnet_detected && drv->connected) {
    status = AS5600_LL_GetRawAngle(&drv->wheel_angle);
    if (status != HAL_OK) {
      return status;
    }
    drv->wheel_position = drv->wheel_angle;
  }

  return HAL_OK;
}

void MX_I2C1_Init(void);

extern I2C_HandleTypeDef hi2c1;

void TryRecoverI2C(HAL_StatusTypeDef status) {
  if(status != HAL_OK) {
    // I2C1->CR1 &= ~(I2C_CR1_PE); //Reset PE bit on I2C control Register
    // HAL_Delay(1);
    // I2C1->CR1 |= I2C_CR1_PE;
    HAL_StatusTypeDef new_status = ResetSteeringWheelDriver();
    
    if(new_status != HAL_OK) {
      HAL_I2C_DeInit(&hi2c1);
      MX_I2C1_Init();
      ResetSteeringWheelDriver();
    }
  }
}

HAL_StatusTypeDef
SteeringWheelDriver_UpdateWheelPosition(SteeringWheelDriver *drv) {
  HAL_StatusTypeDef status;
  status = UpdateMagnetStatus(drv);
  TryRecoverI2C(status);
  if (status != HAL_OK) {
    return status;
    if (!drv->connected) {
      drv->wheel_position = 0;
    }
  }

  if (!drv->magnet_detected) {
    return HAL_ERROR;
  }

  uint16_t angle = 0;
  uint16_t angle_prev = drv->wheel_angle;

  status = AS5600_LL_GetRawAngle(&angle);
  TryRecoverI2C(status);
  if (status != HAL_OK) {
    return status;
  }

  if (angle_prev > REVOLUTION_PREV_MIN && angle < REVOLUTION_NEXT_MAX)
    drv->revolutions++;

  if (angle > REVOLUTION_PREV_MIN && angle_prev < REVOLUTION_NEXT_MAX)
    drv->revolutions--;

  drv->wheel_angle = angle;
  drv->wheel_position =
      drv->revolutions * REVOLUTION_MAX + drv->wheel_angle + 120;

  return HAL_OK;
}
