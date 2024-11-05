/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_custom_hid_if.c
  * @version        : v2.0_Cube
  * @brief          : USB Device Custom HID interface file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usbd_custom_hid_if.h"

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device.
  * @{
  */

/** @addtogroup USBD_CUSTOM_HID
  * @{
  */

/** @defgroup USBD_CUSTOM_HID_Private_TypesDefinitions USBD_CUSTOM_HID_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Defines USBD_CUSTOM_HID_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */

/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Macros USBD_CUSTOM_HID_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Variables USBD_CUSTOM_HID_Private_Variables
  * @brief Private variables.
  * @{
  */

/** Usb HID report descriptor. */

#include "hid_def.h"

__ALIGN_BEGIN static uint8_t CUSTOM_HID_ReportDesc_FS[USBD_CUSTOM_HID_REPORT_DESC_SIZE] __ALIGN_END =
{
  /* USER CODE BEGIN 0 */
    HID_USAGE_PAGE (GENERIC_DESKTOP),
    HID_LOGICAL_MINIMUM1 (0),
    HID_USAGE (JOYSTICK),
    HID_COLLECTION (APPLICATION),
    
      //Joystick Input report
      HID_REPORT_ID (1),
      HID_USAGE_PAGE (SIMULATION),
      HID_USAGE (STEERING),
      HID_LOGICAL_MINIMUM2 (-32768),
      HID_LOGICAL_MAXIMUM2 (32767),
      HID_REPORT_SIZE (16),
      HID_REPORT_COUNT (1),
      HID_INPUT(DATA, VARIABLE, ABSOLUTE),  

      HID_USAGE_PAGE (SIMULATION),
      HID_USAGE (ACCELERATOR),
      HID_USAGE (BRAKE),
      HID_LOGICAL_MINIMUM2 (0),
      HID_LOGICAL_MAXIMUM2 (65535),
      HID_REPORT_SIZE (16),
      HID_REPORT_COUNT (2),
      HID_INPUT (DATA, VARIABLE, ABSOLUTE),  

      HID_USAGE_PAGE (BUTTON),
      HID_USAGE_MINIMUM1(1),
      HID_USAGE_MAXIMUM1(32),
      HID_LOGICAL_MINIMUM1 (0),
      HID_LOGICAL_MAXIMUM1 (1),
      HID_REPORT_SIZE (1),
      HID_REPORT_COUNT (32),
      HID_INPUT (DATA, VARIABLE, ABSOLUTE),

      // PIDStateReport
      HID_USAGE_PAGE (PHYSICAL_DEVICE),
      HID_USAGE (PID_STATE_REPORT),
      HID_COLLECTION (LOGICAL),
        HID_REPORT_ID (2),
        HID_USAGE (DEVICE_PAUSED),
        HID_USAGE (ACTUATORS_ENABLED),
        HID_USAGE (SAFETY_SWITCH),
        HID_USAGE (ACTUATOR_OVERRIDE_SWITCH),
        HID_USAGE (ACTUATOR_POWER),
        HID_LOGICAL_MINIMUM1 (0),
        HID_LOGICAL_MAXIMUM1 (1),
        HID_PHYSICAL_MINIMUM1 (0),
        HID_PHYSICAL_MAXIMUM1 (1),
        HID_REPORT_SIZE (1),
        HID_REPORT_COUNT (5),
        HID_INPUT (DATA, VARIABLE, ABSOLUTE),
        HID_REPORT_COUNT (3),
        HID_INPUT (CONSTANT, VARIABLE, ABSOLUTE),
        0x09, 0x94,             // Usage (Effect Playing)
        HID_LOGICAL_MINIMUM1 (0),
        HID_LOGICAL_MAXIMUM1 (1),
        HID_PHYSICAL_MINIMUM1 (0),
        HID_PHYSICAL_MAXIMUM1 (1),
        HID_REPORT_SIZE (1),
        HID_REPORT_COUNT (1),
        HID_INPUT (DATA, VARIABLE, ABSOLUTE),
        
        0x09, 0x22,             // Usage (Effect Block Index)
        HID_LOGICAL_MINIMUM1 (1),
        HID_LOGICAL_MAXIMUM1 (40),
        HID_PHYSICAL_MINIMUM1 (1),
        HID_PHYSICAL_MAXIMUM1 (40),
        HID_REPORT_SIZE (7),
        HID_REPORT_COUNT (1),
        HID_INPUT (DATA, VARIABLE, ABSOLUTE),
      HID_END_COLLECTION (LOGICAL),

      // SetEffectReport
      0x09, 0x21,           //Usage (Set Effect Report)
      
      HID_COLLECTION (LOGICAL),
        HID_REPORT_ID(1),
        0x09, 0x22,           //  Usage (Effect Block Index)
        HID_LOGICAL_MINIMUM1 (1),
        HID_LOGICAL_MAXIMUM1 (40),
        HID_PHYSICAL_MINIMUM1 (1),
        HID_PHYSICAL_MAXIMUM1 (40),
        HID_REPORT_SIZE (8),
        HID_REPORT_COUNT (1),
        HID_OUTPUT(DATA, VARIABLE, ABSOLUTE),

        0x09, 0x25,           //  Usage (Effect Type)
        HID_COLLECTION (LOGICAL),
          0x09, 0x26,           // USAGE (26) Constant
          0x09, 0x27,           // USAGE (27) Ramp
          0x09, 0x30,           // USAGE (30) Square
          0x09, 0x31,           // USAGE (31) Sine
          0x09, 0x32,           // USAGE (32) Triangle
          0x09, 0x33,           // USAGE (33) Sawtooth Up
          0x09, 0x34,           // USAGE (34) Sawtooth Down
          0x09, 0x40,           // USAGE (40) Spring
          0x09, 0x41,           // USAGE (41) Damper
          0x09, 0x42,           // USAGE (42) Inertia
          0x09, 0x43,           // USAGE (43) Friction
          HID_LOGICAL_MINIMUM1 (1),
          HID_LOGICAL_MAXIMUM1 (11),
          HID_PHYSICAL_MINIMUM1 (1),
          HID_PHYSICAL_MAXIMUM1 (11),
          HID_REPORT_SIZE (8),
          HID_REPORT_COUNT (1),
          HID_OUTPUT(DATA),
        HID_END_COLLECTION (LOGICAL),

        0x09, 0x50,           //    Usage (Duration)
        0x09, 0x54,           //    Usage (Trigger Repeat Interval)
        0x09, 0x51,           //    Usage (Sample Period)
        HID_LOGICAL_MINIMUM2 (0),
        HID_LOGICAL_MAXIMUM2 (32767),
        HID_PHYSICAL_MINIMUM2 (0),
        HID_PHYSICAL_MAXIMUM2 (32767),
        0x66, 0x03, 0x10,     //     Unit (4099)
        0x55, 0xFD,           //     Unit Exponent (253)
        HID_REPORT_SIZE (16),
        HID_REPORT_COUNT (3),
        HID_OUTPUT(DATA, VARIABLE, ABSOLUTE),
        0x55, 0x00,           //     Unit Exponent (0)
        0x66, 0x00, 0x00,     //     Unit (0)
        0x09, 0x52,           //    Usage (Gain)
        HID_LOGICAL_MINIMUM1 (0),
        HID_LOGICAL_MAXIMUM1 (255),
        HID_PHYSICAL_MINIMUM1 (1),
        HID_PHYSICAL_MAXIMUM2 (10000),
        HID_REPORT_SIZE (8),
        HID_REPORT_COUNT (1),
        HID_OUTPUT(DATA, VARIABLE, ABSOLUTE),
        0x09, 0x53,           //    Usage (Trigger Button)
        HID_LOGICAL_MINIMUM1 (1),
        HID_LOGICAL_MAXIMUM1 (8),
        HID_PHYSICAL_MINIMUM1 (1),
        HID_PHYSICAL_MAXIMUM1 (8),
        HID_REPORT_SIZE (8),
        HID_REPORT_COUNT (1),
        HID_OUTPUT(DATA, VARIABLE, ABSOLUTE),
        0x09, 0x55,           //    Usage (Axes Enable)
        HID_COLLECTION (LOGICAL),
          0x05, 0x01,           //        Usage Page (Generic Desktop)
          0x09, 0xc8,           //        USAGE (Steering)
          HID_LOGICAL_MINIMUM1 (0),
          HID_LOGICAL_MAXIMUM1 (1),
          HID_REPORT_SIZE (1),
          HID_REPORT_COUNT (1),
          HID_OUTPUT(DATA, VARIABLE, ABSOLUTE),
        HID_END_COLLECTION (LOGICAL),

        HID_REPORT_COUNT (7), // padding
        0x91, 0x03,           //        Output (Constant, Variable)
      
      HID_END_COLLECTION (LOGICAL),
    
      // SetEnvelopeReport
      0x09, 0x5A,           //Usage (Set Envelope Report)
      HID_COLLECTION (LOGICAL),
      HID_REPORT_ID (2),
      
      0x09, 0x22,           //  Usage (Effect Block Index)
      HID_LOGICAL_MINIMUM1 (1),
      HID_LOGICAL_MAXIMUM1 (40),
      HID_PHYSICAL_MINIMUM1 (1),
      HID_PHYSICAL_MAXIMUM1 (40),
      HID_REPORT_SIZE (8),
      HID_REPORT_COUNT (1),
      HID_OUTPUT(DATA, VARIABLE, ABSOLUTE),
      
      0x09, 0x5B,           //  Usage (Attack Level)
      0x09, 0x5D,           //  Usage (Fade Level)
      HID_LOGICAL_MINIMUM2 (0),
      HID_LOGICAL_MAXIMUM2 (16384),
      HID_PHYSICAL_MINIMUM2 (0),
      HID_PHYSICAL_MAXIMUM2 (16384),
      HID_REPORT_SIZE (16),
      HID_REPORT_COUNT (2),
      HID_OUTPUT(DATA, VARIABLE, ABSOLUTE),
      
      0x09, 0x5C,           //  Usage (Attack Time)
      0x09, 0x5E,           //  Usage (Fade Time)
      0x66, 0x03, 0x10,     //   Unit (1003h) English Linear, Seconds
      0x55, 0xFD,           //   Unit Exponent (FDh) (X10^-3 ==> Milisecond)
      HID_LOGICAL_MAXIMUM2 (32767),
      HID_PHYSICAL_MAXIMUM2 (32767),
      HID_REPORT_SIZE (16),
      HID_OUTPUT(DATA, VARIABLE, ABSOLUTE),
      HID_PHYSICAL_MAXIMUM1 (0),
      0x66, 0x00, 0x00,     //   Unit (0)
      0x55, 0x00,           //   Unit Exponent (0)
      HID_END_COLLECTION (LOGICAL),
    
      // SetConditionReport
      0x09, 0x5F,           //Usage (Set Condition Report)
      HID_COLLECTION (LOGICAL),
      HID_REPORT_ID (3),
      0x09, 0x22,           //  Usage (Effect Block Index)
      HID_LOGICAL_MINIMUM1 (1),
      HID_LOGICAL_MAXIMUM1 (40),
      HID_PHYSICAL_MINIMUM1 (1),
      HID_PHYSICAL_MAXIMUM1 (40),
      HID_REPORT_SIZE (8),
      HID_REPORT_COUNT (1),
      HID_OUTPUT(DATA, VARIABLE, ABSOLUTE),
      
      0x09, 0x23,           //  Usage (Parameter Block Offset)
      HID_LOGICAL_MINIMUM1 (0),
      HID_LOGICAL_MAXIMUM1 (3),
      HID_PHYSICAL_MINIMUM1 (0),
      HID_PHYSICAL_MAXIMUM1 (3),
      HID_REPORT_SIZE (4),
      HID_REPORT_COUNT (1),
      HID_OUTPUT(DATA, VARIABLE, ABSOLUTE),
      
      0x09, 0x58,           //  Usage (Type Specific Block Off...)
      HID_COLLECTION (LOGICAL),
      0x0B, 0x01, 0, 0x0A, 0,  //    Usage (Ordinals: Instance 1)
      0x0B, 0x02, 0, 0x0A, 0,  //    Usage (Ordinals: Instance 2)
      HID_REPORT_SIZE (2),
      HID_REPORT_COUNT (2),
      HID_OUTPUT(DATA, VARIABLE, ABSOLUTE),
      
      HID_END_COLLECTION (LOGICAL),
    
      HID_LOGICAL_MINIMUM2 (-16384),
      HID_LOGICAL_MAXIMUM2 (16384),
      HID_PHYSICAL_MINIMUM2 (-16384),
      HID_PHYSICAL_MAXIMUM2 (16384),
      
      0x09, 0x60,           //  Usage (CP Offset)
      HID_REPORT_SIZE (16),
      HID_REPORT_COUNT (1),
      HID_OUTPUT(DATA, VARIABLE, ABSOLUTE),
    
      HID_LOGICAL_MINIMUM2 (-16384),
      HID_LOGICAL_MAXIMUM2 (16384),
      HID_PHYSICAL_MINIMUM2 (-16384),
      HID_PHYSICAL_MAXIMUM2 (16384),
      
      0x09, 0x61,           //  Usage (Positive Coefficient)
      0x09, 0x62,           //  Usage (Negative Coefficient)
      HID_REPORT_COUNT (2),
      HID_OUTPUT(DATA, VARIABLE, ABSOLUTE),
      
      HID_LOGICAL_MINIMUM2 (0),
      HID_LOGICAL_MAXIMUM2 (16384),
      HID_PHYSICAL_MINIMUM2 (0),
      HID_PHYSICAL_MAXIMUM2 (16384),
      0x09, 0x63,           //  Usage (Positive Saturation)
      0x09, 0x64,           //  Usage (Negative Saturation)
      HID_REPORT_SIZE (16),
      HID_REPORT_COUNT (2),
      HID_OUTPUT(DATA, VARIABLE, ABSOLUTE),
      
      0x09, 0x65,           //  Usage (Dead Band)
      HID_PHYSICAL_MAXIMUM2 (16384),
      HID_REPORT_COUNT (1),
      HID_OUTPUT(DATA, VARIABLE, ABSOLUTE),
      
      HID_END_COLLECTION (LOGICAL),
    
      
      // SetPeriodicReport
    
      0x09, 0x6E,           //Usage (Set Periodic Report)
      HID_COLLECTION (LOGICAL),
      HID_REPORT_ID (4),
      0x09, 0x22,           //  Usage (Effect Block Index)
      HID_LOGICAL_MINIMUM1 (1),
      HID_LOGICAL_MAXIMUM1 (40),
      HID_PHYSICAL_MINIMUM1 (1),
      HID_PHYSICAL_MAXIMUM1 (40),
      HID_REPORT_SIZE (8),
      HID_REPORT_COUNT (1),
      HID_OUTPUT(DATA, VARIABLE, ABSOLUTE),
      0x09, 0x70,           //  Usage (Magnitude)
      HID_LOGICAL_MINIMUM2 (0),
      HID_LOGICAL_MAXIMUM2 (16384),
      HID_PHYSICAL_MINIMUM2 (0),
      HID_PHYSICAL_MAXIMUM2 (16384),
      HID_REPORT_SIZE (16),
      HID_REPORT_COUNT (1),
      HID_OUTPUT(DATA, VARIABLE, ABSOLUTE),
      0x09, 0x6F,           //  Usage (Offset)
      HID_LOGICAL_MINIMUM2 (-16384),
      HID_LOGICAL_MAXIMUM2 (16384),
      HID_PHYSICAL_MINIMUM2 (-16384),
      HID_PHYSICAL_MAXIMUM2 (16384),
        
      HID_REPORT_COUNT (1),
      HID_REPORT_SIZE (16),
      HID_OUTPUT(DATA, VARIABLE, ABSOLUTE),
      0x09, 0x71,           //  Usage (Phase)
      0x66, 0x14, 0x00,     //   Unit (14h) (Eng Rotation, Degrees)
      0x55, 0xFE,           //   Unit Exponent (FEh) (X10^-2)
      HID_LOGICAL_MINIMUM1 (0),
      HID_LOGICAL_MAXIMUM (4, 35999),
      HID_PHYSICAL_MINIMUM1 (0),
      HID_PHYSICAL_MAXIMUM (4, 35999),
      HID_REPORT_SIZE (16),
      HID_REPORT_COUNT (1),
      HID_OUTPUT(DATA, VARIABLE, ABSOLUTE),
      0x09, 0x72,           //  Usage (Period)
      HID_LOGICAL_MINIMUM1 (0),
      HID_LOGICAL_MAXIMUM2 (32767),
      HID_PHYSICAL_MINIMUM1 (0),
      HID_PHYSICAL_MAXIMUM2 (32767),
      0x66, 0x03, 0x10,     //   Unit (1003h) (English Linear, Seconds)
      0x55, 0xFD,           //   Unit Exponent (FDh) (X10^-3 ==> Milisecond)
      HID_REPORT_SIZE (32),
      HID_REPORT_COUNT (1),
      HID_OUTPUT(DATA, VARIABLE, ABSOLUTE),
      HID_UNIT(NONE),
      HID_UNIT_EXPONENT(0),
      HID_END_COLLECTION (LOGICAL),
    
      // SetConstantForceReport
      0x09, 0x73,           //Usage (Set Constant Force Report)
      HID_COLLECTION (LOGICAL),
        HID_REPORT_ID (5),
        0x09, 0x22,           //  Usage (Effect Block Index)
        HID_LOGICAL_MINIMUM1 (1),
        HID_LOGICAL_MAXIMUM1 (40),
        HID_PHYSICAL_MINIMUM1 (1),
        HID_PHYSICAL_MAXIMUM1 (40),
        HID_REPORT_SIZE (8),
        HID_REPORT_COUNT (1),
        HID_OUTPUT(DATA, VARIABLE, ABSOLUTE),

        0x09, 0x70,           //  Usage (Magnitude)
        HID_LOGICAL_MINIMUM2 (-16384),
        HID_LOGICAL_MAXIMUM2 (16384),
        HID_PHYSICAL_MINIMUM2 (-16384),
        HID_PHYSICAL_MAXIMUM2 (16384),
        HID_REPORT_SIZE (16),
        HID_REPORT_COUNT (1),
        HID_OUTPUT(DATA, VARIABLE, ABSOLUTE),

      HID_END_COLLECTION (LOGICAL),
    
      // SetRampForceReport
      0x09, 0x74,           //Usage (Set Ramp Force Report)
      HID_COLLECTION (LOGICAL),
        HID_REPORT_ID (6),
        0x09, 0x22,           //  Usage (Effect Block Index)
        HID_LOGICAL_MINIMUM1 (1),
        HID_LOGICAL_MAXIMUM1 (40),
        HID_PHYSICAL_MINIMUM1 (1),
        HID_PHYSICAL_MAXIMUM1 (40),
        HID_REPORT_SIZE (8),
        HID_REPORT_COUNT (1),
        HID_OUTPUT(DATA, VARIABLE, ABSOLUTE),
        0x09, 0x75,           //  Usage (Ramp Start)
        0x09, 0x76,           //  Usage (Ramp End)
        HID_LOGICAL_MINIMUM2 (-16384),
        HID_LOGICAL_MAXIMUM2 (16384),
        HID_PHYSICAL_MINIMUM2 (-16384),
        HID_PHYSICAL_MAXIMUM2 (16384),
        HID_REPORT_SIZE (16),
        HID_REPORT_COUNT (2),
        HID_OUTPUT(DATA, VARIABLE, ABSOLUTE),
      HID_END_COLLECTION (LOGICAL),
    
      // EffectOperationReport
      0x05, 0x0F,           //Usage Page (Physical Interface)
      0x09, 0x77,           //Usage (Effect Operation Report)
      HID_COLLECTION (LOGICAL),
        HID_REPORT_ID (10),
        0x09, 0x22,           //  Usage (Effect Block Index)
        HID_LOGICAL_MINIMUM1 (1),
        HID_LOGICAL_MAXIMUM1 (40),
        HID_PHYSICAL_MINIMUM1 (1),
        HID_PHYSICAL_MAXIMUM1 (40),
        HID_REPORT_SIZE (8),
        HID_REPORT_COUNT (1),
        HID_OUTPUT(DATA, VARIABLE, ABSOLUTE),
        0x09, 0x78,           //  Usage (Effect Operation)
        HID_COLLECTION (LOGICAL),
          0x09, 0x79,           //    Usage (Op Effect Start)
          0x09, 0x7A,           //    Usage (Op Effect Start Solo)
          0x09, 0x7B,           //    Usage (Op Effect Stop)
          HID_LOGICAL_MINIMUM1 (1),
          HID_LOGICAL_MAXIMUM1 (3),
          HID_REPORT_SIZE (8),
          HID_REPORT_COUNT (1),
          HID_OUTPUT(DATA),
        HID_END_COLLECTION (LOGICAL),
        0x09, 0x7C,           //  Usage (Loop Count)
        HID_LOGICAL_MINIMUM1 (0),
        HID_LOGICAL_MAXIMUM1 (255),
        HID_PHYSICAL_MINIMUM1 (0),
        HID_PHYSICAL_MAXIMUM1 (255),
        HID_OUTPUT(DATA, VARIABLE, ABSOLUTE),
      HID_END_COLLECTION (LOGICAL),
    
      // PIDBlockFreeReport
      0x09, 0x90,           //Usage (PID Block Free Report)
      HID_COLLECTION (LOGICAL),
        HID_REPORT_ID (11),
        0x09, 0x22,           //  Usage (Effect Block Index)
        HID_LOGICAL_MINIMUM1 (1),
        HID_LOGICAL_MAXIMUM1 (40),
        HID_PHYSICAL_MINIMUM1 (1),
        HID_PHYSICAL_MAXIMUM1 (40),
        HID_REPORT_SIZE (8),
        HID_REPORT_COUNT (1),
        HID_OUTPUT(DATA, VARIABLE, ABSOLUTE),
      HID_END_COLLECTION (LOGICAL),

      //PIDDeviceControl
      0x09, 0x96,           //Usage (PID Device Control)
      HID_COLLECTION (LOGICAL),
      HID_REPORT_ID (12),
      0x09, 0x97,           //  Usage (DC Enable Actuators)
      0x09, 0x98,           //  Usage (DC Disable Actuators)
      0x09, 0x99,           //  Usage (DC Stop All Effects)
      0x09, 0x9A,           //  Usage (DC Device Reset)
      0x09, 0x9B,           //  Usage (DC Device Pause)
      0x09, 0x9C,           //  Usage (DC Device Continue)
      HID_LOGICAL_MINIMUM1 (1),
      HID_LOGICAL_MAXIMUM1 (6),
      HID_REPORT_SIZE (8),
      HID_REPORT_COUNT (1),
      HID_OUTPUT(DATA),
      HID_END_COLLECTION (LOGICAL),
    
      // DeviceGainReport
      0x09, 0x7D,           //Usage (Device Gain Report)
      HID_COLLECTION (LOGICAL),
      HID_REPORT_ID (13),
      0x09, 0x7E,           //  Usage (Device Gain)
      HID_LOGICAL_MINIMUM1 (0),
      HID_LOGICAL_MAXIMUM1 (255),
      HID_PHYSICAL_MINIMUM1 (0),
      HID_PHYSICAL_MAXIMUM2 (10000),
      HID_REPORT_SIZE (8),
      HID_REPORT_COUNT (1),
      HID_OUTPUT(DATA, VARIABLE, ABSOLUTE),
      HID_END_COLLECTION (LOGICAL),

      //CreateNewEffectReport
      0x09, 0xAB, // USAGE (Create New Effect Report)
      0xA1, 0x02, // COLLECTION (Logical)
      0x85, 0x05, // REPORT_ID (05)
      0x09, 0x25, // USAGE (Effect Type)
      0xA1, 0x02, // COLLECTION (Logical)
      0x09, 0x26, // USAGE (26)
      0x09, 0x27, // USAGE (27)
      0x09, 0x30, // USAGE (30)
      0x09, 0x31, // USAGE (31)
      0x09, 0x32, // USAGE (32)
      0x09, 0x33, // USAGE (33)
      0x09, 0x34, // USAGE (34)
      0x09, 0x40, // USAGE (40)
      0x09, 0x41, // USAGE (41)
      0x09, 0x42, // USAGE (42)
      0x09, 0x43, // USAGE (43)
      0x25, 0x0B, // LOGICAL_MAXIMUM (0B)
      0x15, 0x01, // LOGICAL_MINIMUM (01)
      0x35, 0x01, // PHYSICAL_MINIMUM (01)
      0x45, 0x0B, // PHYSICAL_MAXIMUM (0B)
      0x75, 0x08, // REPORT_SIZE (08)
      0x95, 0x01, // REPORT_COUNT (01)
      0xB1, 0x00, // FEATURE (Data)
      0xC0, // END COLLECTION ()
      0x05, 0x01, // USAGE_PAGE (Generic Desktop)
      0x09, 0x3B, // USAGE (Byte Count)
      0x15, 0x00, // LOGICAL_MINIMUM (00)
      0x26, 0xFF, 0x01, // LOGICAL_MAXIMUM (511)
      0x35, 0x00, // PHYSICAL_MINIMUM (00)
      0x46, 0xFF, 0x01, // PHYSICAL_MAXIMUM (511)
      0x75, 0x0A, // REPORT_SIZE (0A)
      0x95, 0x01, // REPORT_COUNT (01)
      0xB1, 0x02, // FEATURE (Data,Var,Abs)
      0x75, 0x06, // REPORT_SIZE (06)
      0xB1, 0x01, // FEATURE (Constant,Ary,Abs)
      0xC0, // END COLLECTION ()
    
      // PIDBlockLoadReport
      0x05, 0x0F, // USAGE_PAGE (Physical Interface)
      0x09, 0x89, // USAGE (PID Block Load Report)
      0xA1, 0x02, // COLLECTION (Logical)
      0x85, 0x06, // REPORT_ID (06)
      0x09, 0x22, // USAGE (Effect Block Index)
      0x25, 0x28, // LOGICAL_MAXIMUM (28)
      0x15, 0x01, // LOGICAL_MINIMUM (01)
      0x35, 0x01, // PHYSICAL_MINIMUM (01)
      0x45, 0x28, // PHYSICAL_MAXIMUM (28)
      0x75, 0x08, // REPORT_SIZE (08)
      0x95, 0x01, // REPORT_COUNT (01)
      0xB1, 0x02, // FEATURE (Data,Var,Abs)
      0x09, 0x8B, // USAGE (Block Load Status)
      0xA1, 0x02, // COLLECTION (Logical)
      0x09, 0x8C, // USAGE (Block Load Success)
      0x09, 0x8D, // USAGE (Block Load Full)
      0x09, 0x8E, // USAGE (Block Load Error)
      0x25, 0x03, // LOGICAL_MAXIMUM (03)
      0x15, 0x01, // LOGICAL_MINIMUM (01)
      0x35, 0x01, // PHYSICAL_MINIMUM (01)
      0x45, 0x03, // PHYSICAL_MAXIMUM (03)
      0x75, 0x08, // REPORT_SIZE (08)
      0x95, 0x01, // REPORT_COUNT (01)
      0xB1, 0x00, // FEATURE (Data)
      0xC0, // END COLLECTION ()
      0x09, 0xAC, // USAGE (RAM Pool Available)
      0x15, 0x00, // LOGICAL_MINIMUM (00)
      0x27, 0xFF, 0xFF, 0x00, 0x00, // LOGICAL_MAXIMUM (00 00 FF FF)
      0x35, 0x00, // PHYSICAL_MINIMUM (00)
      0x47, 0xFF, 0xFF, 0x00, 0x00, // PHYSICAL_MAXIMUM (00 00 FF FF)
      0x75, 0x10, // REPORT_SIZE (10)
      0x95, 0x01, // REPORT_COUNT (01)
      0xB1, 0x00, // FEATURE (Data)
      0xC0, // END COLLECTION ()
    
      // PIDPoolReport
      0x09, 0x7F, // USAGE (PID Pool Report)
      0xA1, 0x02, // COLLECTION (Logical)
      0x85, 0x07, // REPORT_ID (07)
      0x09, 0x80, // USAGE (RAM Pool Size)
      0x75, 0x10, // REPORT_SIZE (10)
      0x95, 0x01, // REPORT_COUNT (01)
      0x15, 0x00, // LOGICAL_MINIMUM (00)
      0x35, 0x00, // PHYSICAL_MINIMUM (00)
      0x27, 0xFF, 0xFF, 0x00, 0x00, // LOGICAL_MAXIMUM (00 00 FF FF)
      0x47, 0xFF, 0xFF, 0x00, 0x00, // PHYSICAL_MAXIMUM (00 00 FF FF)
      0xB1, 0x02, // FEATURE (Data,Var,Abs)
      0x09, 0x83, // USAGE (Simultaneous Effects Max)
      0x26, 0xFF, 0x00, // LOGICAL_MAXIMUM (00 FF)
      0x46, 0xFF, 0x00, // PHYSICAL_MAXIMUM (00 FF)
      0x75, 0x08, // REPORT_SIZE (08)
      0x95, 0x01, // REPORT_COUNT (01)
      0xB1, 0x02, // FEATURE (Data,Var,Abs)
      0x09, 0xA9, // USAGE (Device Managed Pool)
      0x09, 0xAA, // USAGE (Shared Parameter Blocks)
      0x75, 0x01, // REPORT_SIZE (01)
      0x95, 0x02, // REPORT_COUNT (02)
      0x15, 0x00, // LOGICAL_MINIMUM (00)
      0x25, 0x01, // LOGICAL_MAXIMUM (01)
      0x35, 0x00, // PHYSICAL_MINIMUM (00)
      0x45, 0x01, // PHYSICAL_MAXIMUM (01)
      0xB1, 0x02, // FEATURE (Data,Var,Abs)
      
      0x75, 0x06, // REPORT_SIZE (06)
      0x95, 0x01, // REPORT_COUNT (01)
      0xB1, 0x03, // FEATURE ( Cnst,Var,Abs)
      0xC0, // END COLLECTION ()

  /* USER CODE END 0 */
  0xC0    /*     END_COLLECTION	             */
};

/* USER CODE BEGIN PRIVATE_VARIABLES */

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Exported_Variables USBD_CUSTOM_HID_Exported_Variables
  * @brief Public variables.
  * @{
  */
extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */
/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_FunctionPrototypes USBD_CUSTOM_HID_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t CUSTOM_HID_Init_FS(void);
static int8_t CUSTOM_HID_DeInit_FS(void);
static int8_t CUSTOM_HID_OutEvent_FS(uint8_t event_idx, uint8_t state);

/**
  * @}
  */

USBD_CUSTOM_HID_ItfTypeDef USBD_CustomHID_fops_FS =
{
  CUSTOM_HID_ReportDesc_FS,
  CUSTOM_HID_Init_FS,
  CUSTOM_HID_DeInit_FS,
  CUSTOM_HID_OutEvent_FS
};

/** @defgroup USBD_CUSTOM_HID_Private_Functions USBD_CUSTOM_HID_Private_Functions
  * @brief Private functions.
  * @{
  */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the CUSTOM HID media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_Init_FS(void)
{
  /* USER CODE BEGIN 4 */
  return (USBD_OK);
  /* USER CODE END 4 */
}

/**
  * @brief  DeInitializes the CUSTOM HID media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_DeInit_FS(void)
{
  /* USER CODE BEGIN 5 */
  return (USBD_OK);
  /* USER CODE END 5 */
}

/**
  * @brief  Manage the CUSTOM HID class events
  * @param  event_idx: Event index
  * @param  state: Event state
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_OutEvent_FS(uint8_t event_idx, uint8_t state)
{
  /* USER CODE BEGIN 6 */
  return (USBD_OK);
  /* USER CODE END 6 */
}

/* USER CODE BEGIN 7 */
/**
  * @brief  Send the report to the Host
  * @param  report: The report to be sent
  * @param  len: The report length
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
/*
static int8_t USBD_CUSTOM_HID_SendReport_FS(uint8_t *report, uint16_t len)
{
  return USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, report, len);
}
*/
/* USER CODE END 7 */

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

