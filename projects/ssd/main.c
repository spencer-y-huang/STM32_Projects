#include <stdint.h>
#include "stm32g0xx.h"

void main(void) {

	// enable GPIO_B clock
	RCC->IOPENR |= (1 << RCC_IOPENR_GPIOBEN_Pos);

	// dummy reads
	volatile uint32_t* dummy;
	dummy = RCC->IOPENR;
	dummy = RCC->IOPENR;

	// Reset the modes of PB0-PB7
	GPIOB->MODER &= ~(0x3 << GPIO_MODER_MODE0_Pos);
	GPIOB->MODER &= ~(0x3 << GPIO_MODER_MODE1_Pos);
	GPIOB->MODER &= ~(0x3 << GPIO_MODER_MODE2_Pos);
	GPIOB->MODER &= ~(0x3 << GPIO_MODER_MODE3_Pos);
	GPIOB->MODER &= ~(0x3 << GPIO_MODER_MODE4_Pos);
	//GPIOB->MODER &= ~(0x3 << GPIO_MODER_MODE5_Pos);
	GPIOB->MODER &= ~(0x3 << GPIO_MODER_MODE6_Pos);
	GPIOB->MODER &= ~(0x3 << GPIO_MODER_MODE7_Pos);
	GPIOB->MODER &= ~(0x3 << GPIO_MODER_MODE8_Pos);
	// Set the modes of PB0-PB7 to output
	GPIOB->MODER |= (0x1 << GPIO_MODER_MODE0_Pos);
	GPIOB->MODER |= (0x1 << GPIO_MODER_MODE1_Pos);
	GPIOB->MODER |= (0x1 << GPIO_MODER_MODE2_Pos);
	GPIOB->MODER |= (0x1 << GPIO_MODER_MODE3_Pos);
	GPIOB->MODER |= (0x1 << GPIO_MODER_MODE4_Pos);
	//GPIOB->MODER |= (0x1 << GPIO_MODER_MODE5_Pos);
	GPIOB->MODER |= (0x1 << GPIO_MODER_MODE6_Pos);
	GPIOB->MODER |= (0x1 << GPIO_MODER_MODE7_Pos);
	GPIOB->MODER |= (0x1 << GPIO_MODER_MODE8_Pos);

	GPIOB->ODR |= (1 << 0);
	GPIOB->ODR |= (1 << 1);
	GPIOB->ODR |= (1 << 2);
	GPIOB->ODR |= (1 << 3);
	GPIOB->ODR |= (1 << 4);
	GPIOB->ODR |= (1 << 6);
	GPIOB->ODR |= (1 << 7);
	while(1) {
		// Toggle PB0.
		for(uint32_t i = 0; i < 750000; i++);
		GPIOB->ODR ^= (1 << 8);
	}
}
