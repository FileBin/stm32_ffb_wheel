/************************************************************************
Title:	  as5600.c - Driver for AMS AS5600 12-Bit Programmable Contactless
                                         Potentiometer
Author:   Nicholas Morrow <nickhudspeth@gmail.com>
File:     as5600.c
Software: STM32Fxxx_HAL_Driver, CMSIS-CORE
Hardware: STM32Fxxx
License:  The MIT License (MIT)
Usage:    Refer to the header file as5600.h for a description of the routines.
          See also example test_as5600.c, if available.
LICENSE:
    Copyright (C) 2018 Nicholas Morrow

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
    deal in the Software without restriction, including without limitation the
    rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
************************************************************************/

/**********************    INCLUDE DIRECTIVES    ***********************/

#include "as5600_ll.h"
#include <stdint.h>

#define I2C_TIMEOUT 20
#define I2C_ADDRESS AS5600_SHIFTED_SLAVE_ADDRESS

extern I2C_HandleTypeDef hi2c1;

/**********************    GLOBAL VARIABLES    ***********************/

/*******************    FUNCTION IMPLEMENTATIONS    ********************/
HAL_StatusTypeDef AS5600_LL_Init(AS5600_InitStruct *init_struct) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t pwm = 0;
  /* Set configuration defaults for uninitialized values. */
  if (!(init_struct->PositiveRotationDirection)) {
    init_struct->PositiveRotationDirection = AS5600_DIR_CW;
  }
  if (!(init_struct->LowPowerMode)) {
    init_struct->LowPowerMode = AS5600_POWER_MODE_DEFAULT;
  }
  if (!(init_struct->Hysteresis)) {
    init_struct->Hysteresis = AS5600_HYSTERESIS_DEFAULT;
  }
  if (!(init_struct->OutputMode)) {
    init_struct->OutputMode = AS5600_OUTPUT_STAGE_DEFAULT;
  }
  if (!(init_struct->PWMFrequency)) {
    init_struct->PWMFrequency = AS5600_PWM_FREQUENCY_DEFAULT;
  }
  if (!(init_struct->SlowFilter)) {
    init_struct->SlowFilter = AS5600_SLOW_FILTER_DEFAULT;
  }
  if (!(init_struct->FastFilterThreshold)) {
    init_struct->FastFilterThreshold = AS5600_FAST_FILTER_DEFAULT;
  }
  if (!(init_struct->WatchdogTimer)) {
    init_struct->WatchdogTimer = AS5600_WATCHDOG_DEFAULT;
  }

  uint8_t confRegister[2] = {0};
  /* Write configuration settings.
     Do this in single write instead of using functions below to avoid
     overhead of multiple calls to HAL_I2C_Mem_Write */
  switch (init_struct->LowPowerMode) {
  case AS5600_POWER_MODE_NOM:
    confRegister[1] &= ~((1UL << 1) | (1UL << 0));
    break;
  case AS5600_POWER_MODE_LPM1:
    confRegister[1] |= (1UL << 0);
    confRegister[1] &= ~(1UL << 1);
    break;
  case AS5600_POWER_MODE_LPM2:
    confRegister[1] |= (1UL << 1);
    confRegister[1] &= (1UL << 0);
    break;
  case AS5600_POWER_MODE_LPM3:
    confRegister[1] |= ((1UL << 1) | (1UL << 0));
    break;
  default:
    /* Invalid low power mode specified */
    status = HAL_ERROR;
    return status;
  }
  switch (init_struct->Hysteresis) {
  case AS5600_HYSTERESIS_OFF:
    confRegister[1] &= ~((1UL << 3) | (1UL << 2));
    break;
  case AS5600_HYSTERESIS_1LSB:
    confRegister[1] |= (1UL << 2);
    confRegister[1] &= ~(1UL << 3);
    break;
  case AS5600_HYSTERESIS_2LSB:
    confRegister[1] &= ~(1UL << 2);
    confRegister[1] |= (1UL << 3);
    break;
  case AS5600_HYSTERESIS_3LSB:
    confRegister[1] |= ((1UL << 3) | (1UL << 2));
    break;
  default:
    /* Invalid hysteresis mode specified */
    status = HAL_ERROR;
    return status;
  }
  switch (init_struct->OutputMode) {
  case AS5600_OUTPUT_STAGE_FULL:
    confRegister[1] &= ~((1UL << 5) | (1UL << 4));
    break;
  case AS5600_OUTPUT_STAGE_REDUCED:
    confRegister[1] |= (1UL << 4);
    confRegister[1] &= ~(1UL << 5);
    break;
  case AS5600_OUTPUT_STAGE_PWM:
    confRegister[1] &= (1UL << 4);
    confRegister[1] |= (1UL << 5);
    pwm = 1;
    break;
  default:
    /* Invalid output mode specified */
    status = HAL_ERROR;
    return status;
  }
  if (pwm) {
    switch (init_struct->PWMFrequency) {
    case AS5600_PWM_FREQUENCY_115HZ:
      confRegister[1] &= ~((1UL << 7) | (1UL << 6));
      break;
    case AS5600_PWM_FREQUENCY_230HZ:
      confRegister[1] |= (1UL << 6);
      confRegister[1] &= ~(1UL << 7);
      break;
    case AS5600_PWM_FREQUENCY_460HZ:
      confRegister[1] &= ~(1UL << 6);
      confRegister[1] |= (1UL << 7);
      break;
    case AS5600_PWM_FREQUENCY_920HZ:
      confRegister[1] |= ((1UL << 7) | (1UL << 6));
      break;
    default:
      /* Invalid PWM frequency specified. */
      status = HAL_ERROR;
      return status;
    }
  }
  switch (init_struct->SlowFilter) {
  case AS5600_SLOW_FILTER_16X:
    confRegister[0] &= ~((1UL << 1) | (1UL << 0));
    break;
  case AS5600_SLOW_FILTER_8X:
    confRegister[0] |= (1UL << 0);
    confRegister[0] &= ~(1UL << 1);
    break;
  case AS5600_SLOW_FILTER_4X:
    confRegister[0] &= ~(1UL << 0);
    confRegister[0] |= (1UL << 1);
    break;
  case AS5600_SLOW_FILTER_2X:
    confRegister[0] |= ((1UL << 1) | (1UL << 0));
    break;
  default:
    /* Invalid slow filter mode specified */
    status = HAL_ERROR;
    return status;
  }
  switch (init_struct->FastFilterThreshold) {
  case AS5600_FAST_FILTER_SLOW_ONLY:
    confRegister[0] &= ~((1UL << 4) | (1UL << 3) | (1UL << 2));
    break;
  case AS5600_FAST_FILTER_6LSB:
    confRegister[0] &= ~((1UL << 4) | (1UL << 3));
    confRegister[0] |= (1UL << 2);
    break;
  case AS5600_FAST_FILTER_7LSB:
    confRegister[0] &= ~((1UL << 4) | (1UL << 2));
    confRegister[0] |= (1UL << 3);
    break;
  case AS5600_FAST_FILTER_9LSB:
    confRegister[0] &= ~(1UL << 4);
    confRegister[0] |= ((1UL << 3) | (1UL << 2));
    break;
  case AS5600_FAST_FILTER_18LSB:
    confRegister[0] &= ~((1UL << 3) | (1UL << 2));
    confRegister[0] |= (1UL << 4);
    break;
  case AS5600_FAST_FILTER_21LSB:
    confRegister[0] &= ~(1UL << 3);
    confRegister[0] |= ((1UL << 4) | (1UL << 2));
    break;
  case AS5600_FAST_FILTER_24LSB:
    confRegister[0] &= ~(1UL << 2);
    confRegister[0] |= ((1UL << 4) | (1UL << 3));
    break;
  case AS5600_FAST_FILTER_10LSB:
    confRegister[0] |= ((1UL << 4) | (1UL << 3) | (1UL << 2));
    break;
  default:
    /* Invalid slow filter mode specified */
    status = HAL_ERROR;
    return status;
  }
  switch (init_struct->WatchdogTimer) {
  case AS5600_WATCHDOG_OFF:
    confRegister[0] &= ~(1UL << 6);
    break;
  case AS5600_WATCHDOG_ON:
    confRegister[0] |= (1UL << 6);
    break;
  default:
    /* Invalid watchdog state specified */
    status = HAL_ERROR;
    return status;
  }
  status =
      HAL_I2C_Mem_Write(&hi2c1, I2C_ADDRESS, AS5600_REGISTER_CONF_HIGH,
                        I2C_MEMADD_SIZE_8BIT, confRegister, 2, I2C_TIMEOUT);
  return status;
}

HAL_StatusTypeDef AS5600_LL_GetRawAngle(uint16_t *const angle) {
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t data[2] = {0};

  status = HAL_I2C_Mem_Read(&hi2c1, I2C_ADDRESS, AS5600_REGISTER_RAW_ANGLE_HIGH,
                            I2C_MEMADD_SIZE_8BIT, data, 2, I2C_TIMEOUT);
  if (status == HAL_OK) {
    *angle = ((data[0] << 8) | data[1]) & AS5600_12_BIT_MASK;
  }

  return status;
}

HAL_StatusTypeDef AS5600_LL_GetMagnetStatus(uint8_t *const stat) {
  return HAL_I2C_Mem_Read(&hi2c1, I2C_ADDRESS, AS5600_REGISTER_STATUS,
                          I2C_MEMADD_SIZE_8BIT, stat, 1, I2C_TIMEOUT);
}
