#include "motor.h"

#include "ffb_engine.h"
#include "main.h"
#include "util.h"

#define MOTOR_MIN_FORCE 2

extern TIM_HandleTypeDef htim3;
void updateMotor(void) {
  int32_t force = FFBEngine_CalculateForce();
  int8_t direction = sign(force);
  force = abs(force);
  force -= MOTOR_MIN_FORCE;

  if (force > 0) {
    int positivePinState = direction > 0 ? 1 : 0;
    HAL_GPIO_WritePin(MotorPositive_GPIO_Port, MotorPositive_Pin,
                      positivePinState);
    HAL_GPIO_WritePin(MotorNegative_GPIO_Port, MotorNegative_Pin,
                      1 - positivePinState);
  } else {
    force = 0;
    HAL_GPIO_WritePin(MotorNegative_GPIO_Port, MotorNegative_Pin,
                      GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MotorPositive_GPIO_Port, MotorPositive_Pin,
                      GPIO_PIN_RESET);
  }

  TIM_OC_InitTypeDef sConfigOC = {0};

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.Pulse = constrain(abs(force), 0, 16383);

  while (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) !=
         HAL_OK) {
    HAL_Delay(1);
  }
}
