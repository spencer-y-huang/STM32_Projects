CC = arm-none-eabi-gcc
CFLAGS=-mcpu=cortex-m0plus -mthumb -nostdlib
CPPFLAGS=-DSTM32G070xx \
	-I../vendor/CMSIS_6/CMSIS/Core/Include \
	-I../vendor/STM32CubeG0/Drivers/CMSIS/Device/ST/STM32G0xx/Include 

LINKER_FILE=linker_script.ld 
LDFLAGS=-T $(LINKER_FILE) -lgcc

all: timer.elf

timer.elf: main.o startup.o system_stm32g0xx.o
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ $(LDFLAGS) -o timer.elf

main.o: main.c
	$(CC) $(CFLAGS) $(CPPFLAGS) main.c -c
	
startup.o: startup.c
	$(CC) $(CFLAGS) $(CPPFLAGS) startup.c -c

system_stm32g0xx.o: ../vendor/STM32CubeG0/Drivers/CMSIS/Device/ST/STM32G0xx/Source/Templates/system_stm32g0xx.c
	$(CC) $(CFLAGS) $(CPPFLAGS) ../vendor/STM32CubeG0/Drivers/CMSIS/Device/ST/STM32G0xx/Source/Templates/system_stm32g0xx.c -c

.PHONY: clean
clean:
	$(RM) *.o *.elf

PROGRAMMER=openocd
PROGRAMMER_FLAGS=-f interface/stlink.cfg -f target/stm32g0x.cfg

flash: timer.elf
	$(PROGRAMMER) $(PROGRAMMER_FLAGS) -c "program timer.elf verify reset exit"
