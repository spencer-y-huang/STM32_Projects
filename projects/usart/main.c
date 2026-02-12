#include <stdint.h>
#include <stdio.h>
#include "stm32g0xx.h"
#include "ringbuf.h"

#define LED_PIN 5

#define RINGBUF_SIZE (128)

volatile char rb_buf[RINGBUF_SIZE + 1];
ringbuf rb = {
	len: RINGBUF_SIZE,
	buf: rb_buf,
	pos: 0,
	ext: 0
};
volatile int newline = 0;

void start_timer(void);

// Configure MCU clock
// Target: 64 MHz 
void setup_clock(void) {
	// set wait states for flash (need 2 for 64 MHz)
	FLASH->ACR &= (~FLASH_ACR_LATENCY_Msk);
	FLASH->ACR |= FLASH_ACR_LATENCY_2;

	// enable instruction prefetch
	FLASH->ACR |= FLASH_ACR_PRFTEN;

	// disable PLL clock
	RCC->CR &= ~RCC_CR_PLLON;
	// wait until PLL is stopped
	while (RCC->CR & RCC_CR_PLLRDY);

	// Set PLL parameters
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSI;				// use HSI as input to PLL - 16 MHz
	RCC->PLLCFGR |= 0x1 << RCC_PLLCFGR_PLLM_Pos;		// PLLM - div. by 2 - now 8 MHz
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN;
	RCC->PLLCFGR |= 0x10 << RCC_PLLCFGR_PLLN_Pos;		// PLLN - mult. by 16 - now 128 Mhz

	RCC->PLLCFGR |= RCC_PLLCFGR_PLLR_0; 			// PLLR - div. by 2 - now 64 MHz
	
	// enable PLL clock
	RCC->CR |= RCC_CR_PLLON;
	// wait until PLL is on
	while (!(RCC->CR & RCC_CR_PLLRDY));

	// enable PLLR clock output
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;
	// enable PLLR as system clock
	RCC->CFGR |= RCC_CFGR_SW_PLLRCLK;

	// wait until PLLR is used as clock
	while (!(RCC->CFGR & RCC_CFGR_SWS_PLLRCLK));
}

void main(void) {
	setup_clock();

	// set USART2 to use SYSCLK
	RCC->CCIPR |= (1 << RCC_CCIPR_USART2SEL_Pos);

	// enable GPIO_A, USART2 clock
	RCC->IOPENR |= (1 << RCC_IOPENR_GPIOAEN_Pos);
	RCC->APBENR1 |= RCC_APBENR1_USART2EN;
	
	// Reset the mode of PA5
	GPIOA->MODER &= ~(0x3 << GPIO_MODER_MODE5_Pos);
	// Set PA5 to output
	GPIOA->MODER |= (0x1 << GPIO_MODER_MODE5_Pos);

	// Reset the mode of PA2, PA3
	GPIOA->MODER &= ~(0x3 << GPIO_MODER_MODE2_Pos);
	GPIOA->MODER &= ~(0x3 << GPIO_MODER_MODE3_Pos);

	// Set PA2, PA3 to alternate function mode
	GPIOA->MODER |= (0x2 << GPIO_MODER_MODE2_Pos);
	GPIOA->MODER |= (0x2 << GPIO_MODER_MODE3_Pos);

	// Set PA2, PA3 to use AF1
	GPIOA->AFR[0] |= (0x1 << GPIO_AFRL_AFSEL2_Pos);
	GPIOA->AFR[0] |= (0x1 << GPIO_AFRL_AFSEL3_Pos);

	// Reset PA2, PA3 output types
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT2;
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT3;

	// Set PA2, PA3 to use high speed
	GPIOA->OSPEEDR &= ~(0x3 << GPIO_OSPEEDR_OSPEED2_Pos);
	GPIOA->OSPEEDR &= ~(0x3 << GPIO_OSPEEDR_OSPEED3_Pos);
	GPIOA->OSPEEDR &= (0x2 << GPIO_OSPEEDR_OSPEED2_Pos);
	GPIOA->OSPEEDR &= (0x2 << GPIO_OSPEEDR_OSPEED3_Pos);

	uint16_t uartdiv = 64000000 / 115200;
	//uint16_t uartdiv = 16000000 / 9600;
	// set USART2 baud rate
	USART2->BRR = uartdiv;

	// enable USART2
	USART2->CR1 |= (USART_CR1_RE | USART_CR1_UE);

	// Enable NVIC interrupt for USART2_IRQn
	NVIC_SetPriority(USART2_IRQn, 0x02);
	NVIC_EnableIRQ(USART2_IRQn);

	// handle transmit enable (?)
	USART2->CR1 |= USART_CR1_TE;
	USART2->CR1 &= ~USART_CR1_TE;
	//while(!(USART2->ISR & USART_ISR_TEACK));
	USART2->CR1 |= USART_CR1_TE;

	// enable USART2 interrupts
	USART2->CR1 |= (USART_CR1_RXNEIE_RXFNEIE);

	GPIOA->ODR ^= (1 << 5);
	for(int i = 0; i < 1000; i++);
	GPIOA->ODR ^= (1 << 5);

	while(1) { 
		while (newline == 0) {
			__WFI();
		}
		GPIOA->ODR &= ~(1 << 5);
		while (rb.pos != rb.ext) {
			putchar(ringbuf_read(&rb));
		}
		printf("\n");
		newline = 0;
	};
}

void usart2_handler(void) {
	if(USART2->ISR & USART_ISR_RXNE_RXFNE){
		GPIOA->ODR |= (1 << 5);
		char c = USART2->RDR;
		ringbuf_write(&rb, c);
		if(c == '\r') {
			newline = 1;
		}
	}
}
