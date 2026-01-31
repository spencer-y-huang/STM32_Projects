#!/bin/bash

openocd -f interface/stlink.cfg -f target/stm32g0x.cfg -c "program blink.elf verify reset exit"
