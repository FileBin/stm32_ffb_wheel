#include "blink.h"
#include "motor.h"
#include "input.h"

void user_main(void) {
  blink_led();
  process_input();
  updateMotor();
}

