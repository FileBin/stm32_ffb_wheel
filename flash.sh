#!/usr/bin/env bash
# -*- coding: utf-8 -*-

cd build/Debug
st-flash --debug write firmware.bin 0x8000000 