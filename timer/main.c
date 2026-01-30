#include <stdint.h>
#include "stm32g0xx.h"

#define LED_PIN 5

void start_timer(void);

void main(void) {
	// Enable TIM14 clock
	RCC->APBENR2 |= (1 << RCC_APBENR2_TIM14EN_Pos);

	// Enable NVIC interrupt for TIM14_IRQn
	NVIC_SetPriority(TIM14_IRQn, 0x03);
	NVIC_EnableIRQ(TIM14_IRQn);


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

	start_timer();

	while(1) { }
}

// for now, hardcoded to use TIM14
void start_timer(void) {
	// turn off timer
	TIM14->CR1 &= ~(TIM_CR1_CEN);

	// reset timer
	RCC->APBRSTR2 |=  (RCC_APBRSTR2_TIM14RST);
	RCC->APBRSTR2 &= ~(RCC_APBRSTR2_TIM14RST);

	// set prescaler/autoreload registers
	TIM14->PSC = 16000000/1000;
	TIM14->ARR = 1000; // 1 second

	// set update event to reset timer
	TIM14->EGR |= TIM_EGR_UG;
	// enable interrupts
	TIM14->DIER |= TIM_DIER_UIE;
	// enable timer
	TIM14->CR1 |= TIM_CR1_CEN;
}

void stop_timer(void) {
	// disable timer
	TIM14->CR1 &= ~(TIM_CR1_CEN);
	// clear interrupt flag, just in case
	TIM14->SR &= ~(TIM_SR_UIF);
}

void tim14_handler(void) {
	// handle timer 'update' interrupt fault
	if (TIM14->SR & TIM_SR_UIF) {
		// reset the interrupt
		TIM14->SR &= ~(TIM_SR_UIF);
		// toggle the LED output pin
		GPIOA->ODR ^= (1 << LED_PIN);
	}
}
