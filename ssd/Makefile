CC = arm-none-eabi-gcc
CFLAGS=-mcpu=cortex-m0plus -mthumb -nostdlib
CPPFLAGS=-DSTM32G070xx \
	-I../vendor/CMSIS_6/CMSIS/Core/Include \
	-I../vendor/STM32CubeG0/Drivers/CMSIS/Device/ST/STM32G0xx/Include 

LINKER_FILE=linker_script.ld 
LDFLAGS=-T $(LINKER_FILE) -lgcc

all: ssd.elf

ssd.elf: main.o startup.o system_stm32g0xx.o
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ $(LDFLAGS) -o ssd.elf

main.o: main.c
	$(CC) $(CFLAGS) $(CPPFLAGS) main.c -c
	
startup.o: startup.c
	$(CC) $(CFLAGS) $(CPPFLAGS) startup.c -c

system_stm32g0xx.o: ../vendor/STM32CubeG0/Drivers/CMSIS/Device/ST/STM32G0xx/Source/Templates/system_stm32g0xx.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -c

# debug!
debug: CFLAGS += -DDEBUG -gdwarf-2 -g3
debug: all

# clean!
.PHONY: clean
clean:
	$(RM) *.o *.elf

## flashing program to board

PROGRAMMER=openocd
PROGRAMMER_FLAGS=-f interface/stlink.cfg -f target/stm32g0x.cfg

flash: ssd.elf
	$(PROGRAMMER) $(PROGRAMMER_FLAGS) -c "program ssd.elf verify reset exit"
