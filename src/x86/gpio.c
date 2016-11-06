#include "gpio.h"

#include <stdbool.h>
#include <stdint.h>

// TODO(ckitagawa): This whole implementation works and is fast but is bad due to the need for
// sourcing configurations. Figure out an alternative. Compared to registers it is memory
// inefficient but it saves having to define fake registers and stores the same information.

// TODO(ckitagawa): Move this to a configuration file which is included in the x86 shared header.
#define MAX_PORTS 6
#define MAX_PINS 16

static GPIOSettings pin_settings[MAX_PORTS * MAX_PINS];
static uint8_t gpio_pin_input_value[MAX_PORTS * MAX_PINS];

// Determines if an GPIOAddress is valid based on the defined number of ports and pins.
static bool prv_is_address_valid(const GPIOAddress *address) {
  return !(address->port >= MAX_PORTS || address->pin >= MAX_PINS);
}

// Determines if a GPIOState is valid based on the enums.
static bool prv_is_state_valid(const GPIOState *state) { return !(*state >= NUM_GPIO_STATE); }

// Determines if a GPIOSettings is valid based on the enums.
static bool prv_are_settings_valid(const GPIOSettings *settings) {
  return !(settings->direction >= NUM_GPIO_DIR || settings->state >= NUM_GPIO_STATE ||
           settings->resistor >= NUM_GPIO_RES || settings->alt_function >= NUM_GPIO_ALTFN);
}

static uint32_t prv_get_index(GPIOAddress *address) {
  return address->port * MAX_PORTS + address->pin;
}

bool gpio_init() {
  // TODO(ckitagawa): Check if MAX_PORTS and MAX_PINS get defined if not fail as the configuration
  // is bad.
  GPIOSettings default_settings = {.direction = GPIO_DIR_IN,
                                   .state = GPIO_STATE_LOW,
                                   .resistor = GPIO_RES_NONE,
                                   .alt_function = GPIO_ALTFN_NONE };
  for (uint32_t i = 0; i < MAX_PORTS * MAX_PINS; i++) {
    pin_settings[i] = default_settings;
    gpio_pin_input_value[i] = 0;
  }
  return true;
}

bool gpio_init_pin(GPIOAddress *address, GPIOSettings *settings) {
  if (!prv_is_address_valid(address) || !prv_are_settings_valid(settings)) {
    return false;
  }

  pin_settings[prv_get_index(address)] = *settings;
  return true;
}

bool gpio_set_pin_state(GPIOAddress *address, GPIOState state) {
  if (!prv_is_address_valid(address) || !prv_is_state_valid(&state)) {
    return false;
  }

  pin_settings[prv_get_index(address)].state = state;
  return true;
}

bool gpio_toggle_state(GPIOAddress *address) {
  if (!prv_is_address_valid(address)) {
    return false;
  }

  uint32_t index = prv_get_index(address);
  if (pin_settings[index].state == GPIO_STATE_LOW) {
    pin_settings[index].state = GPIO_STATE_HIGH;
  } else {
    pin_settings[index].state = GPIO_STATE_LOW;
  }
  return true;
}

bool gpio_get_value(GPIOAddress *address, GPIOState *state) {
  if (!prv_is_address_valid(address)) {
    return false;
  }

  uint32_t index = prv_get_index(address);

  // Behave how hardware does when the direction is set to out.
  if (pin_settings[index].direction == GPIO_DIR_OUT) {
    *state = pin_settings[index].state;
  } else {
    *state = gpio_pin_input_value[index];
  }
  return true;
}
