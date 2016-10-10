#pragma once

// GPIO HAL Interface

#include <stdint.h>

// GPIO GPIO "address" to be used to change that pin's settings
// Use bytes instead of words to save on memory also convenient for MSP430
// compatibility
typedef struct GPIOAddress {
  const uint8_t port;
  const uint8_t pin;
} GPIOAddress;

// Enums size varies by compiler only garuntee is that they will be big enough
// to store their defined number of items. For this particular case since they
// are small they are likely (although far from certain) that they will be
// sizeof(int) unless a --short-enums flag is passed to the compiler

// For setting the direction of the pin
typedef enum {
  GPIO_DIR_IN = 0,
  GPIO_DIR_OUT,
} GPIODir;

// For setting the output value of the pin
typedef enum {
  GPIO_STATE_LOW = 0,
  GPIO_STATE_HIGH,
} GPIOState;

// For setting the internal pull-up/pull-down resistor (Turn off if a hardware
// PUPDR is enabled). Different on MSP and STM for MSP RES_UP and RES_DOWN both
// set GPIO_REN to 1. Could make this RES_ON, RES_OFF and make pullup and
// pulldown vary with the GPIO_VAL the same way it works for the MSP430
typedef enum {
  GPIO_RES_NONE = 0,
  GPIO_RES_PULLUP,
  GPIO_RES_PULLDOWN,
} GPIORes;

// For setting the alternate function on the pin
// Applicable to STM, on MSP just set the GPIO_SEL to 1 unless GPIO_AF0 is
// passed
// On STM if an GPIO_AF is passed set GPIO_MODER to 2 instead of IN or OUT,
// ANALOG
// sets to 3
typedef enum {
  GPIO_AF_0 = 0,
  GPIO_AF_1,
  GPIO_AF_2,
  GPIO_AF_3,
  GPIO_AF_4,
  GPIO_AF_5,
  GPIO_AF_6,
  GPIO_AF_7,
  GPIO_AF_ANALOG,  // Clever way to make DIR analog for STM as GPIO_AF and
                   // Analog
                   // are mutually exclusive
} GPIOAF;

// Ignored settings:
// Speed - should be set when a pin is initialized and not touched again
// Output type - very rarely will we want open drain, unless we have shared
// input or wired OR gates
// Reset and Lock setting on STM

// Address is separated to save memory and make the interface cleaner
// Struct is 4 bytes and will fit in one memory register.
typedef struct GPIOSettings {
  GPIODir dir;
  GPIOState state;
  GPIORes res;
  GPIOAF alt;
} GPIOSettings;

// Initializes GPIO globally, sets all pins to lowest power mode
void gpio_init();

// Initializes a GPIO pin
void gpio_init_pin(GPIOAddress *address, GPIOSettings *settings);

// Set the pin by address
void gpio_set_pin_state(GPIOAddress *address, GPIOState *state);

// Toggles the output state of the pin
void gpio_toggle_state(GPIOAddress *address);

// Gets the value of the input register for a pin
GPIOState gpio_get_value(GPIOAddress *address);
