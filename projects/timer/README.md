debouncing resource: https://www.ganssle.com/debouncing.htm

it seems like it's actually kinda bad practice to tie interrupts to the rising/falling edge of 
a bouncy switch. whoops.
- this is because if you tie the interrupt to rising/falling edges, when the switch bounces
  it'll fire the interrupt multiple times

you can also
- rising/falling edge -> disable interrupt on pin, start polling on that input
- sample the input until stable, then flag the button as pressed
- disable polling, re-enable interrupt
you tie up a timer *some* of the time with this approach.

alternatively, just use a timer and an external variable accessible from the timer
interrupt + main code
- use the timer to sample the input at a set interval
- when the input stabilizes, set the external variable accordingly
the downside of this is that you tie up a timer.
