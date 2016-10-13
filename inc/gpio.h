#pragma once

// GPIO HAL Interface

#include <stdint.h>

// GPIO address to be used to change that pin's settings
typedef struct GPIOAddress {
  const uint8_t port;
  const uint8_t pin;
} GPIOAddress;

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

// For setting the internal pull-up/pull-down resistor
typedef enum {
  GPIO_RES_NONE = 0,
  GPIO_RES_PULLUP,
  GPIO_RES_PULLDOWN,
} GPIORes;

// For setting the alternate function on the pin
typedef enum {
  GPIO_AF_0 = 0,
  GPIO_AF_1,
  GPIO_AF_2,
  GPIO_AF_3,
  GPIO_AF_4,
  GPIO_AF_5,
  GPIO_AF_6,
  GPIO_AF_7,
  GPIO_AF_ANALOG,
} GPIOAF;

// GPIO settings for setting the value of a pin
typedef struct GPIOSettings {
  GPIOAddress address;
  GPIODir direction;
  GPIOState state;
  GPIORes resistor;
  GPIOAF alt_function;
} GPIOSettings;

// Initializes GPIO globally, sets all pins to lowest power mode
void gpio_init();

// Initializes a GPIO pin
void gpio_init_pin(GPIOSettings *settings);

// Set the pin state by address
void gpio_set_pin_state(GPIOAddress *address, GPIOState *state);

// Toggles the output state of the pin
void gpio_toggle_state(GPIOAddress *address);

// Gets the value of the input register for a pin
GPIOState gpio_get_value(GPIOAddress *address);
