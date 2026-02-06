#include <stdint.h>
#include "stm32g0xx.h"

#define LED_PIN 5

void start_timer(void);

void main(void) {

	// Configure MCU clock
	// Target: 64 MHz (the maximum for this chip)
	
	// set wait states for flash (need 2 for 64 MHz)
	FLASH->ACR->LATENCY &= (~FLASH_ACR_LATENCY_MSK);
	FLASH->ACR->LATENCY |= FLASH_ACR_LATENCY_2;

	// set instruction prefetch
	FLASH->ACR->PRFTEN |= FLASH_ACR_PRFTEN;

	// disable PLL clock
	RCC->CR &= ~RCC_CR_PLLON;
	// wait until PLL is stopped
	while (RCC->CR & RCC_CR_PLLRDY);

	// Set PLL parameters
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSI;				// use HSI as input to PLL
	RCC->PLLCFGR |= 0x8 << RCC_PLLCFGR_PLLN_Pos;  // PLL - mult. by 4
	RCC->PLLCFGR |= 0x1 << RCC_PLLCFGR_PLLM_Pos;	// PLL - div. by 2
	
	// enable PLL clock
	RCC->CR |= RCC_CR_PLLON;
	// enable PLLR as system clock
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;

	// Enable TIM14 clock
	RCC->APBENR2 |= (1 << RCC_APBENR2_TIM14EN_Pos);

	// enable GPIO_A, GPIO_C clock
	RCC->IOPENR |= (1 << RCC_IOPENR_GPIOAEN_Pos);
	RCC->IOPENR |= (1 << RCC_IOPENR_GPIOCEN_Pos);

	// Link PC13 to EXTI13
	EXTI->EXTICR[3] &= ~(0x7 << EXTI_EXTICR4_EXTI13_Pos);
	EXTI->EXTICR[3] |= (0x2 << EXTI_EXTICR4_EXTI13_Pos);

	// enable rising/falling triggers on EXTI13
	EXTI->RTSR1 |= (1 << EXTI_RTSR1_RT13_Pos);
	EXTI->FTSR1 |= (1 << EXTI_FTSR1_FT13_Pos);

	// enable interrupt on EXTI13
	EXTI->IMR1 |= (1 << EXTI_IMR1_IM13_Pos);

	// Enable NVIC interrupt for TIM14, EXTI4_15
	NVIC_SetPriority(TIM14_IRQn, 0x03);
	NVIC_EnableIRQ(TIM14_IRQn);
	NVIC_SetPriority(EXTI4_15_IRQn, 0x02);
	NVIC_EnableIRQ(EXTI4_15_IRQn);

	// Reset the mode of PA5
	GPIOA->MODER &= ~(0x3 << GPIO_MODER_MODE5_Pos);
	// Set PA5 to output
	GPIOA->MODER |= (0x1 << GPIO_MODER_MODE5_Pos);

	// Reset the mode of PC13 (this also sets the mode to input)
	GPIOC->MODER &= ~(0x3 << GPIO_MODER_MODE13_Pos);
	// Set PC13 to pull-up
	GPIOC->PUPDR |= (0x1 << GPIO_MODER_MODE13_Pos);

	start_timer();

	while(1) {};
}

// for now, hardcoded to use TIM14
void start_timer(void) {
	// turn off timer
	TIM14->CR1 &= ~(TIM_CR1_CEN);

	// reset timer
	RCC->APBRSTR2 |=  (RCC_APBRSTR2_TIM14RST);
	RCC->APBRSTR2 &= ~(RCC_APBRSTR2_TIM14RST);

	// set prescaler/autoreload registers
	TIM14->PSC = 16000000/1000; // defaults to HSI which is 16 MHz
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

void exti4_15_handler(void) {

	if (EXTI->FPR1 & EXTI_FPR1_FPIF13) {
		// reset the interrupt
		EXTI->FPR1 |= (EXTI_FPR1_FPIF13);
		TIM14->ARR = 250; // 0.25 seconds
	} else if (EXTI->RPR1 & EXTI_RPR1_RPIF13) {
		// reset the interrupt
		EXTI->RPR1 |= ~(EXTI_RPR1_RPIF13);
		TIM14->ARR = 1000; // 1 second
	}

	// reset the timer
	TIM14->EGR |= TIM_EGR_UG;
}
