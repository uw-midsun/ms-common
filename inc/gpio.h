#pragma once

// GPIO HAL Interface

#include <stdbool.h>
#include <stdint.h>

// GPIO Pin "address" to be used to change that pin's settings
// Use bytes instead of words to save on memory also convenient for MSP430
// compatibility
typedef struct GPIOAddress {
  const uint8_t gpio_port;
  const uint8_t gpio_pin;
} GPIOAddress;

// For setting the direction of the pin
typedef enum {
  PIN_DIR_IN = 0,
  PIN_DIR_OUT,
} PIN_DIR;

// For setting the output value of the pin
typedef enum {
  PIN_STATE_LOW = 0,
  PIN_STATE_HIGH,
} PIN_STATE;

// For setting the internal pull-up/pull-down resistor (Turn off if a hardware
// PUPDR is enabled). Different on MSP and STM for MSP RES_UP and RES_DOWN both
// set PIN_REN to 1. Could make this RES_ON, RES_OFF and make pullup and
// pulldown vary with the PIN_VAL the same way it works for the MSP430
typedef enum {
  PIN_RES_OFF = 0,
  PIN_RES_PULLUP,
  PIN_RES_PULLDOWN,
} PIN_RES;

// For setting the alternate function on the pin
// Applicable to STM, on MSP just set the PIN_SEL to 1 unless PIN_AF0 is passed
// On STM if an PIN_AF is passed set PIN_MODER to 2 instead of IN or OUT, ANALOG
// sets to 3
typedef enum {
  PIN_AF_0 = 0,
  PIN_AF_1,
  PIN_AF_2,
  PIN_AF_3,
  PIN_AF_4,
  PIN_AF_5,
  PIN_AF_6,
  PIN_AF_7,
  PIN_AF_ANALOG,  // Clever way to make DIR analog for STM as PIN_AF and Analog
                  // are mutually exclusive
} PIN_AF;

// Ignored settings:
// Speed - should be set when a pin is initialized and not touched again
// Output type - very rarely will we want open drain, unless we have shared
// input or wired OR gates
// Reset and Lock setting on STM

// A 3 byte struct to set the values for a pin
// Could separate out the address from the settings
typedef struct GPIOSettings {
  const GPIOAddress addr;  // 2 bytes
  PIN_DIR dir;             // 1 bit
  PIN_STATE state;         // 1 bit
  PIN_RES res;             // 2 bits (one unused option)
  PIN_AF alt;  // 4 bits (no padding needed if the compiler is smart about enums
               // 7 unused options)
} GPIOSettings;

// Initializes GPIO globally, sets all pins to lowest power mode
void gpio_init();

// Initializes a GPIO pin
void gpio_init_pin(GPIOSettings *settings);

// Toggles the output state of the pin
void gpio_toggle_state(GPIOAddress *address);

// Gets the value of the input register for a pin
bool gpio_get_value(GPIOAddress *address);
