#include "motor.h"
#include "config.h"

#include "ffb_engine.h"
#include "main.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "util.h"
#include <stdint.h>
#include "ffb_axis.h"

#define MOTOR_MIN_FORCE 2

extern TIM_HandleTypeDef htim3;

int motorDirection = 0;
int prevMotorDirection = 0;

uint32_t motorChangeDirectionTime = 0;

void updateMotor(void) {
  uint32_t time = HAL_GetTick();
  int32_t force = FFBEngine_CalculateForce() / 4;

  int motorPositive = force > 0;
  int motorNegative = force < 0;

  force = abs(force);
  force -= MOTOR_MIN_FORCE;

  if (force <= 0) {
    motorPositive = 0;
    motorNegative = 0;
  }

  HAL_GPIO_WritePin(MotorPositive_Pin_GPIO_Port, MotorPositive_Pin, motorPositive);
  HAL_GPIO_WritePin(MotorNegative_Pin_GPIO_Port, MotorNegative_Pin, motorNegative);

  TIM_OC_InitTypeDef sConfigOC = {0};

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.Pulse = constrain(abs(force), 0, 4095);

  // if (time < motorChangeDirectionTime + CHANGE_DIRECTION_SAFE_TIME) {
  //   sConfigOC.Pulse = 0;
  // } else if (newMotorDirection != motorDirection && force > 0) {
  //   motorChangeDirectionTime = time;
  //   motorDirection = newMotorDirection;
  // }

  // if (motorChangeDirectionTime > time) {
  //   motorChangeDirectionTime = time;
  // }

  // if (time > motorChangeDirectionTime + CHANGE_DIRECTION_SAFE_TIME / 2) {
  //   if (prevMotorDirection != motorDirection) {

  //   }
  //   prevMotorDirection = motorDirection;
  // }

  while (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) !=
         HAL_OK) {
    HAL_Delay(1);
  }
}
