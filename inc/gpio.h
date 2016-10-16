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
  GPIO_ALTFN_NONE = 0,
  GPIO_ALTFN_0,
  GPIO_ALTFN_1,
  GPIO_ALTFN_2,
  GPIO_ALTFN_3,
  GPIO_ALTFN_4,
  GPIO_ALTFN_5,
  GPIO_ALTFN_6,
  GPIO_ALTFN_7,
  GPIO_ALTFN_ANALOG,
} GPIOAltFn;

// GPIO settings for setting the value of a pin
typedef struct GPIOSettings {
  GPIODir direction;
  GPIOState state;
  GPIORes resistor;
  GPIOAltFn alt_function;
} GPIOSettings;

// Initializes GPIO globally, sets all pins to lowest power mode
void gpio_init();

// Initializes a GPIO pin
void gpio_init_pin(GPIOAddress *address, GPIOSettings *settings);

// Set the pin state by address
void gpio_set_pin_state(GPIOAddress *address, GPIOState *state);

// Toggles the output state of the pin
void gpio_toggle_state(GPIOAddress *address);

// Gets the value of the input register for a pin
GPIOState gpio_get_value(GPIOAddress *address);
