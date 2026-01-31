#include <stdint.h>

#define PERIPHERAL_BASE (0x40000000U)
#define IO_BASE (PERIPHERAL_BASE + 0x10000000U)
#define AHB_BASE (PERIPHERAL_BASE + 0x20000U)
#define RCC_BASE (AHB_BASE + 0x1000U)

#define RCC_AHBIOENR_OFFSET (0x34U)
#define RCC_AHBIOENR ((volatile uint32_t*) (RCC_BASE + RCC_AHBIOENR_OFFSET))
#define RCC_AHBIOENR_GPIOAEN (0x00U)

#define GPIOA_BASE (IO_BASE + 0x0U)
#define GPIO_MODER_OFFSET (0x00U)
#define GPIOA_MODER ((volatile uint32_t*) (GPIOA_BASE + GPIO_MODER_OFFSET))
#define GPIO_MODER_MODER5 (10U)

#define GPIOA_ODR_OFFSET (0x14U)
#define GPIOA_ODR ((volatile uint32_t*) (GPIOA_BASE + GPIOA_ODR_OFFSET))

#define GPIOA_BSRR_OFFSET (0x18U)
#define GPIOA_BSRR ((volatile uint32_t*) (GPIOA_BASE + GPIOA_ODR_OFFSET))

#define LED_PIN 5

void main(void) {
	// enable GPIO_A clock
	*RCC_AHBIOENR |= (1 << RCC_AHBIOENR_GPIOAEN);

	// dummy reads
	volatile uint32_t* dummy;
	dummy = *(RCC_AHBIOENR);
	dummy = *(RCC_AHBIOENR);

	// Reset the mode of PA5
	*GPIOA_MODER &= ~(0x3 << GPIO_MODER_MODER5);
	// Set PA5 to output
	*GPIOA_MODER |= (0x1 << GPIO_MODER_MODER5);

	while(1) {
		// Toggle PA5.
		*GPIOA_ODR ^= (1 << LED_PIN);
		for(uint32_t i = 0; i < 1000000; i++);
	}
}
