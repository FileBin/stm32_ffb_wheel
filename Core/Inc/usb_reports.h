#include <stdint.h>

#ifndef __INCLUDE_USB_REPORT__
#define __INCLUDE_USB_REPORT__

#define HID_REPORT typedef struct __attribute__((packed, aligned(1)))

// Input reports
#define JOYSTICK_INPUT_REPORT_ID 1
HID_REPORT JoystickInputReport
{
  uint8_t id;
  int16_t steering;
  uint16_t accelerator;
  uint16_t brake;
  uint32_t buttons;
}
JoystickInputReport;

void JoystickInputReport_Init(JoystickInputReport *report);


#define PID_STATE_REPORT_ID 2
HID_REPORT PIDStateReport
{

  uint8_t id;
  struct
  {
    uint8_t devicePaused : 1;
    uint8_t actuatorsEnabled : 1;
    uint8_t actuatorPower : 1;
    uint8_t actuatorOverrideSwitch : 1;
  };

  struct
  {
    uint8_t effectPlaying : 1;
    uint8_t effectBlockIndex : 7;
  };
}
PIDStateReport;

void PIDStateReport_Init(PIDStateReport *report);


// Output Reports
// ...

// Feature Reports
#define CREATE_NEW_EFFECT_REPORT_ID 5

typedef enum PID_EffectType
{
  ET_CONSTANT_FORCE = 1,
  ET_RAMP,
  ET_SQUARE,
  ET_SINE,
  ET_TRIANGLE,
  ET_SAWTOOTH_UP,
  ET_SAWTOOTH_DOWN,
  ET_SPRING,
  ET_DAMPER,
  ET_INERTIA,
  ET_FRICTION,
} PID_EffectType;

HID_REPORT PID_CreateNewEffectReport
{
  uint8_t id;
  uint8_t effectType;
  uint8_t byteCount;
}
PID_CreateNewEffectReport;

#define PID_BLOCK_LOAD_REPORT_ID 6

typedef enum PID_BlockLoadStatus
{
  BLOCK_LOAD_SUCCESS = 1,
  BLOCK_LOAD_FULL = 2,
  BLOCK_LOAD_ERROR = 3,
} PID_BlockLoadStatus;

HID_REPORT PID_BlockLoadReport
{
  uint8_t id;
  uint8_t effectBlockIndex;
  uint8_t blockLoadStatus;
  uint16_t ramPoolAvailable;
}
PID_BlockLoadReport;

#define PID_POOL_FEATURE_REPORT_ID 7
#define PID_DEVICE_MANAGED_POOL 1
#define PID_SHARED_PARAMETER_BLOCKS 1

HID_REPORT PID_PoolFeatureReport
{
  uint8_t id;
  uint16_t ramPoolSize;
  uint8_t maxEffects;
  uint8_t deviceManagedPool : 1;
  uint8_t sharedParameterBlocks : 1;
}
PID_PoolFeatureReport;

void PIDPoolFeatureReport_Init(PID_PoolFeatureReport *report);
#endif
