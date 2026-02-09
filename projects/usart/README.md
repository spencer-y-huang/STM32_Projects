UART

UART2 can connect to:
* ST-Link
* CN10 pin 6/34 

RCC: defined in APBENR2
	
UART2_TX: PA2, AF1
UART2_RX: PA3, AF1

IO pins 0-7 are configured via GPIOx_AFRL, and are set to AF0 on startup.

- configure pins as output/input (GPIOx_MODER)
	- TX: transmit, output
	- RX: receive, input

- connect to alternate function (GPIOx_AFRL or GPIOx_AFRH)
- set type, pull-up/down, output speed (GPIOx_OTYPER, GPIOx_PUPDR, GPIOx_OSPEEDER)
	- type: open-drain (?)
	- pupdr: ?
	- output speed: very high speed (?)
- set as alternate (GPIOx_MODER)

- enable USART
	- USART2_CR1 (RE, TE, UE)
		- receive enable, transmit enable, usart enable

most common UART settings:
- 8 bits per byte
- no parity checks
- 1 "stop" bit

default for STM32070RB seems different...
- 8 bits per byte
- 1 "start" bit
- no parity checks
- n "stop" bits - this needs to be set in USART2_CR2 
	- default is 1 stop bit!
- FIFO or not? (default: not FIFO)

section 26.5.5 in the reference manual seems to describe some of this

- also set baud rate (USART_BRR)
	- 9600?
	- USART_BRR seems to be a clock divider rather than setting it directly; need
		to look it up

Read data:
- USART_RDR, USART_ISR_RXNE (read not empty)

Transmit data:
- USART_TDR, USART_ISR_TXE (transit empty)
