# LED driver

Allows a user to use a button to switch between 5 light patterns on a WS2812B LED strip.

This code requires the ESP-IDF libraries. 

### Engineering Elements Encorporated:
  1) GPIO's: The button is connected to a GPIO with an internal PULLUP resistor. When the button is pressed, the GPIO is connected to GND which triggers a negative-edge interrupt.
  
  2) Timer: A timer divides the main clock to count to 0.1 seconds to determine when to send the next string of commands to the LEDs. 
  
  3) Interrupts: The button and timer are both connected to the interrupts. The IRQ handlers perform the following functions:
      
      a) Button: There is a state machine that determines whicho color pattern to display. When the button is pressed, the state machine advances to the next color pattern.
      
      b) Timer: The timer interrupt handler clears the interrupt, resets the timer, and then sends the next "frame" to the LEDs.

  4) Protocols: The WS2812B LED strips use a protocol to program the LED's. For more information about this protocol, [click here](https://cdn-shop.adafruit.com/datasheets/WS2812B.pdf).
  I implemented this protocol using the [ESP32 RMT library](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/rmt.html). This involved creating RMT items for a logical one or logical zero (based on WS2812B datasheet) and then transmitting these RMT items to the LED driver.
