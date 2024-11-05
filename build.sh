#!/usr/bin/env bash
# -*- coding: utf-8 -*-

cp STM32F103C8Tx_FLASH.ld.bak STM32F103C8Tx_FLASH.ld

cd build/Debug

ninja
arm-none-eabi-objcopy -O binary stm32_ffb_wheel.elf firmware.bin