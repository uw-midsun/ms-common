#pragma once

#include <stdint.h>

#include "gpio.h"
#include "status.h"

typedef void (*gpio_it_callback)(void);

typedef enum {
  GPIO_IT_TYPE_INTERRUPT = 0,
  GPIO_IT_TYPE_EVENT,
  NUM_GPIO_IT_TYPE,
} GPIOITType;

typedef enum {
  GPIO_IT_EDGE_RISING = 0,
  GPIO_IT_EDGE_RISING_FALLING,
  GPIO_IT_EDGE_FALLING,
  NUM_GPIO_IT_EDGE,
} GPIOITEdge;

typedef struct GPIOITSettings {
  GPIOITType type;
  GPIOITEdge edge;
  uint8_t priority;
} GPIOITSettings;

// Sets the gpio interrupt and callback function for the provided address. The pin still needs to be
// configured manually. If the device uses a multiplexer on ports and you override the interrupt for
// the previous pin no warning is provided. However, the global status message will note this to aid
// in debugging.
StatusCode gpio_it_enable(GPIOAddress *address, GPIOITSettings *settings,
                          gpio_it_callback callback);
