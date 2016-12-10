#include "gpio_it.h"

#include <stdbool.h>
#include <stdint.h>

#include "interrupt.h"
#include "status.h"
#include "x86/gpio_cpu.h"
#include "x86/interrupt_cpu.h"

static bool prv_is_address_valid(const GPIOAddress *address) {
  return !(address->port >= NUM_GPIO_CPU_PORTS || address->pin >= NUM_GPIO_CPU_PINS);
}

static uint32_t prv_get_index(GPIOAddress *address) {
  return address->port * NUM_GPIO_CPU_PORTS + address->pin;
}

static uint8_t s_callback_id[NUM_GPIO_CPU_PORTS * NUM_GPIO_CPU_PINS];

StatusCode gpio_it_enable(GPIOAddress *address, InterruptSettings *settings,
                          interrupt_callback callback) {
  if (!prv_is_address_valid(address) || settings->priority >= NUM_INTERRUPT_PRIORITY ||
      settings->type >= NUM_INTERRUPT_TYPE || settings->edge >= NUM_INTERRUPT_EDGE) {
    return status_code(STATUS_CODE_INVALID_ARGS);
  }

  interrupt_cpu_init();

  uint8_t id;
  status_ok_or_return(interrupt_cpu_register(settings, callback, &id));
  s_callback_id[prv_get_index(address)] = id;

  return STATUS_CODE_OK;
}

StatusCode gpio_it_sw_interrupt(GPIOAddress *address) {
  if (prv_is_address_valid(address)) {
    interrupt_cpu_raise(s_callback_id[prv_get_index(address)]);
    return STATUS_CODE_OK;
  }
  return status_code(STATUS_CODE_INVALID_ARGS);
}
