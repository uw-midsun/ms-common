#pragma once

#include "gpio.h"
#include "interrupt.h"
#include "status.h"

// Sets the gpio interrupt and callback function for the provided address. The pin still needs to be
// configured manually. If the device uses a multiplexer on ports and you override the interrupt for
// the previous pin no warning is provided. However, the global status message will note this to aid
// in debugging.
StatusCode gpio_it_enable(GPIOAddress *address, InterruptSettings *settings,
                          interrupt_callback callback);

// Triggers the interrupt in software rather than in hardware.
StatusCode gpio_it_sw_interrupt(GPIOAddress *address);
