#!/bin/bash

openocd -f interface/stlink.cfg -f target/stm32g0x.cfg -c "program button.elf verify reset exit"
