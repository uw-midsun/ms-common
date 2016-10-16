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
  GPIO_ALTF_0 = 0,
  GPIO_ALTF_1,
  GPIO_ALTF_2,
  GPIO_ALTF_3,
  GPIO_ALTF_4,
  GPIO_ALTF_5,
  GPIO_ALTF_6,
  GPIO_ALTF_7,
  GPIO_ALTF_ANALOG,
  GPIO_ALTF_NONE,
} GPIOAltF;

// GPIO settings for setting the value of a pin
typedef struct GPIOSettings {
  GPIODir direction;
  GPIOState state;
  GPIORes resistor;
  GPIOAltF alt_function;
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
