#include <stdint.h>

#define PERIPHERAL_BASE (0x40000000U)
#define IO_BASE (PERIPHERAL_BASE + 0x10000000U)
#define AHB_BASE (PERIPHERAL_BASE + 0x20000U)
#define RCC_BASE (AHB_BASE + 0x1000U)

#define RCC_AHBIOENR_OFFSET (0x34U)
#define RCC_AHBIOENR ((volatile uint32_t*) (RCC_BASE + RCC_AHBIOENR_OFFSET))
#define RCC_AHBIOENR_GPIOAEN (0x00U)
#define RCC_AHBIOENR_GPIOCEN (0x02U)

#define GPIOA_BASE (IO_BASE + 0x0U)
#define GPIO_MODER_OFFSET (0x00U)
#define GPIOA_MODER ((volatile uint32_t*) (GPIOA_BASE + GPIO_MODER_OFFSET))
#define GPIO_MODER_MODER5 (10U)

#define GPIOC_BASE (IO_BASE + 0x00000800U)
#define GPIOC_MODER ((volatile uint32_t*) (GPIOC_BASE + GPIO_MODER_OFFSET))
#define GPIO_MODER_MODER13 (26U)
#define GPIO_PUPDR_OFFSET (0x0CU)
#define GPIOC_PUPDR ((volatile uint32_t*) (GPIOC_BASE + GPIO_PUPDR_OFFSET))

#define GPIOA_ODR_OFFSET (0x14U)
#define GPIOA_ODR ((volatile uint32_t*) (GPIOA_BASE + GPIOA_ODR_OFFSET))

#define GPIOC_IDR_OFFSET (0x10)
#define GPIOC_IDR ((volatile uint32_t*) (GPIOC_BASE + GPIOC_IDR_OFFSET))

#define LED_PIN 5

void main(void) {
	// enable GPIO_A, GPIO_C clock
	*RCC_AHBIOENR |= (1 << RCC_AHBIOENR_GPIOAEN);
	*RCC_AHBIOENR |= (1 << RCC_AHBIOENR_GPIOCEN);

	// dummy reads
	volatile uint32_t* dummy;
	dummy = (volatile uint32_t*) *(RCC_AHBIOENR);
	dummy = (volatile uint32_t*) *(RCC_AHBIOENR);

	// Reset the mode of PA5
	*GPIOA_MODER &= ~(0x3 << GPIO_MODER_MODER5);
	// Set PA5 to output
	*GPIOA_MODER |= (0x1 << GPIO_MODER_MODER5);
	
	// Reset the mode of PC13 (this also sets the mode to input)
	*GPIOC_MODER &= ~(0x3 << GPIO_MODER_MODER13);
	// Set PC13 to pull-up
	*GPIOC_PUPDR |= (0x1 << GPIO_MODER_MODER13);

	while(1) {
		// set PA5 depending on the value of PC13
		int PC13 = (*GPIOC_IDR >> 13);
		*GPIOA_ODR = (~(PC13 & 1) << LED_PIN);
	}
}
