arm-none-eabi-gcc main.c startup.c -T linker_script.ld -o blink.elf -mcpu=cortex-m0 -mthumb -nostdlib
