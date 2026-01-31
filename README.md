Projects using the STM32 NUCLE-G070RB board.

* [X] blink - very simple LED blink program
* [X] button - when the user button is pressed, activate the LED.
* [o] timer - blink, but using a timer interrupt than a while loop. 
	* [X] Blinks faster when the button is held down.
	* [X] Use interrupts tied into the button to change the timer interval.
	* [ ] Implement debouncing for the button interrupt.
* [ ] UART - implement UART tutorial to allow board to send messages to host PC
* [.] ssd - use a Diligent PmodSSD seven segment display
	* [X] can turn on segments of display
	* [ ] write code to configure the display to show a specific number/character
	* [ ] combine with timer to count up from 0, user-button pauses count
* [ ] temp - use an Analog Devices ADT7420 I2C temperature sensor
	* [ ] read temperature 
	* [ ] print temperature to host PC
	* [ ] display temperature on ssd
* [ ] compass - use a Memsic MMC3416xPJ compass
	* [ ] light up LED when facing north
	* [ ] print heading to host PC
	* [ ] print heading to ssd

using [a tutorial](https://kleinembedded.com/stm32-without-cubeide-part-1-the-bare-necessities/) 
from the Klein Embedded blog.

also looking at [this tutorial](https://embedded.fm/blog/ese101) for some
additional context - even though it's a different architecture/chip, it still covers the
low-level stuff

another [tutorial](https://vivonomicon.com/category/stm32_baremetal_examples/page/2/) - more data points aren't
bad, especially since I'm being insanely stubborn and not using CubeMX

microcontroller: STM32G070RB
microcontroller reference: [here](https://www.st.com/en/microcontrollers-microprocessors/stm32g070rb.html?ecmp=tt9470_gl_link_feb2019&rt=db&id=DB2196#documentation)
