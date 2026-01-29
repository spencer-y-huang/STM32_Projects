#include <stdint.h>
#include "stm32g0xx.h"

#define LED_PIN 5

void main(void) {

	// set hse bypass???? 
	RCC->CR |= RCC_CR_HSEBYP_Msk | RCC_CR_HSEON_Msk;
	//while(!(RCC->CR & RCC_CR_HSERDY_Msk));

	// enable GPIO_A clock
	RCC->IOPENR |= (1 << RCC_IOPENR_GPIOAEN_Pos);

	// dummy reads
	volatile uint32_t* dummy;
	dummy = RCC->IOPENR;
	dummy = RCC->IOPENR;

	// Reset the mode of PA5
	GPIOA->MODER &= ~(0x3 << GPIO_MODER_MODE5_Pos);
	// Set PA5 to output
	GPIOA->MODER |= (0x1 << GPIO_MODER_MODE5_Pos);

	while(1) {
		// Toggle PA5.
		GPIOA->ODR ^= (1 << LED_PIN);
		for(uint32_t i = 0; i < 1000000; i++);
	}
}
