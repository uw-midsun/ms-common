#pragma once

// GPIO HAL Interface

#include <stdbool.h>
#include <stdint.h>

// IO Pin "address" to be used to change that pin's settings
// Use bytes instead of words to save on memory also convenient for MSP430
// compatibility
typedef struct IOAddr {
  const uint8_t IOPort;  // Could be an enum (A-F) for stm32f0xx
  const uint8_t IOPin;
} IOAddr;

// For setting the direction of the pin
typedef enum { PIN_IN = 0; PIN_OUT; }
PIN_DIR;

// For setting the output value of the pin
typedef enum { PIN_LOW = 0; PIN_HIGH; }
PIN_VAL;

// For setting the internal pull-up/pull-down resistor (Turn off if a hardware
// PUPDR is enabled)
// Different on MSP and STM for MSP RES_UP and RES_DOWN both set PIN_REN to 1
// Could make this RES_ON, RES_OFF and make pullup and pulldown vary with the
// PIN_VAL as it works
// for the MSP430
typedef enum { RES_OFF = 0; RES_UP; RES_DOWN; }
PIN_RES;

// For setting the alternate function on the pin
// Applicable to STM, on MSP just set the PIN_SEL to 1 unless AF0 is passed
// On STM if an AF is passed set PIN_MODER to 2 instead of IN or OUT, ANALOG
// sets to 3
typedef enum {
  AF0 = 0; AF1; AF2; AF3; AF4; AF5; AF6; AF7;
  ANALOG;  // Clever way to make DIR analog for STM as AF and Analog are
           // mutually exclusive
}
PIN_AF;

// Ignored settings:
// Speed - should be set when a pin is initialized and not touched again
// Output type - very rarely will we want open drain, unless we have shared
// input or wired OR gates
// Reset and Lock setting on STM

// A struct to set the values for a pin
typedef struct IOSettings {
  const IOAddr addr;  // The necessity of making this const is debatable
  PIN_DIR dir;
  PIN_VAL val;
  PIN_RES res;
  PIN_AF alt;
} IOSettings;

// Initializes a pin with all of the settings entered
void gpio_init(IOSettings *settings);

// Updates the gpio based on the values passed in the struct
// (just boilerplate for calling init again to make the user interface better
// could maybe be avoid by calling gpio_set and gpio_unset instead of init and
// deinit?)
void gpio_update(IOSettings *settings);

// Sets the pin to the lowest power mode possible
void gpio_deinit(IO_Addr *address);

// Alternatively for toggles pass a IOSettings object and toggle the value in
// the struct as well,
// this may be confusing but also allows tracking of the settings of a given pin
// nicely without
// the need for functions to query the state of a pin. The solution presented
// here assumes there
// is very limited use cases for getting the value of a setting for a pin and
// ignores it.

// Toggles the direction of the pin
void gpio_toggle_dir(IOAddr *address);

// Toggles the output of the pin
void gpio_toggle_val(IOAddr *address);

// Sets the PUPDR (toggling this doesn't really make sense)
void gpio_set_res(IOAddr *address, const PIN_RES res);

// Sets the alt function (toggling this doesn't make sense)
void gpio_set_af(IOAddr *address, const PIN_AF alt);

// Gets the value of the input register for a pin
bool gpio_get_value(IOAddr *address);
