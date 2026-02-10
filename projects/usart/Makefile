CC = arm-none-eabi-gcc
CFLAGS=-mcpu=cortex-m0plus -mthumb --specs=nano.specs
CPPFLAGS=-DSTM32G070xx \
	-I../../vendor/CMSIS_6/CMSIS/Core/Include \
	-I../../vendor/STM32CubeG0/Drivers/CMSIS/Device/ST/STM32G0xx/Include 

LINKER_FILE=linker_script.ld 
LDFLAGS=-T $(LINKER_FILE)

all: usart.elf

usart.elf: main.o startup.o syscalls.o system_stm32g0xx.o
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ $(LDFLAGS) -o usart.elf

main.o: main.c
	$(CC) $(CFLAGS) $(CPPFLAGS) main.c -c
	
startup.o: startup.c
	$(CC) $(CFLAGS) $(CPPFLAGS) startup.c -c

syscalls.o: syscalls.c
	$(CC) $(CFLAGS) $(CPPFLAGS) syscalls.c -c

system_stm32g0xx.o: ../../vendor/STM32CubeG0/Drivers/CMSIS/Device/ST/STM32G0xx/Source/Templates/system_stm32g0xx.c
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

flash: usart.elf
	$(PROGRAMMER) $(PROGRAMMER_FLAGS) -c "program usart.elf verify reset exit"
