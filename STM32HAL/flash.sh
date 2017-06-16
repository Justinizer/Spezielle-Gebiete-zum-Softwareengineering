#!/bin/bash
JLinkExe -device STM32F103C8 -If SWD -speed 4000 -CommandFile flashBinary.jlink
