#include "main.h"

#define BLINK_INTERVAL 1000

GPIO_PinState g_prev_led_state = 0;

void blink_led(void)
{
  GPIO_PinState led_state =
      (HAL_GetTick() % BLINK_INTERVAL) < (BLINK_INTERVAL / 2) ? GPIO_PIN_RESET
                                                              : GPIO_PIN_SET;
  if (led_state != g_prev_led_state)
  {
    HAL_GPIO_WritePin(board_led_GPIO_Port, board_led_Pin, led_state);
    g_prev_led_state = led_state;
  }
}