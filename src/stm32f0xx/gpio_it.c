#include "gpio_it.h"

#include <stdint.h>
#include <stdlib.h>

#include "gpio.h"
#include "interrupt.h"
#include "misc.h"
#include "status.h"
#include "stm32f0xx.h"
#include "stm32f0xx/exti.h"
#include "stm32f0xx/gpio_mcu.h"
#include "stm32f0xx/nvic.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_syscfg.h"

static interrupt_callback s_gpio_it_callbacks[NUM_GPIO_PINS];

StatusCode gpio_it_enable(GPIOAddress *address, InterruptSettings *settings,
                          interrupt_callback callback) {
  if (address->port >= NUM_GPIO_PORTS || address->pin >= NUM_GPIO_PINS ||
      settings->edge >= NUM_INTERRUPT_EDGE || settings->type >= NUM_INTERRUPT_TYPE ||
      settings->priority >= NUM_INTERRUPT_PRIORITY) {
    return status_code(STATUS_CODE_INVALID_ARGS);
  }

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  SYSCFG_EXTILineConfig(address->port, address->pin);

  EXTISettings exti_settings = {.type = (EXTIType)settings->type,
                                .edge = (EXTIEdge)settings->edge };
  exti_enable(address->pin, &exti_settings);

  if (s_gpio_it_callbacks[address->pin] != NULL) {
    status_msg(STATUS_CODE_OK, "Overrode existing interrupt.");
  }
  s_gpio_it_callbacks[address->pin] = callback;

  if (address->pin < 2) {
    // NVIC channel for EXTI pins 0, 1 is 5.
    nvic_enable(5, settings->priority);
  } else if (address->pin < 4) {
    // NVIC channel for EXTI pins 2, 3 is 6.
    nvic_enable(6, settings->priority);
  } else {
    // NVIC channel for EXTI pins 4-15 is 7.
    nvic_enable(7, settings->priority);
  }
  return STATUS_CODE_OK;
}

void prv_run_callbacks(uint8_t lower_bound, uint8_t upper_bound) {
  EXTIPending pending;
  for (int i = lower_bound; i < upper_bound; i++) {
    exti_get_pending(i, &pending);
    if (pending && s_gpio_it_callbacks[i] != NULL) {
      s_gpio_it_callbacks[i]();
    }
    exti_clear_pending(i);
  }
}

void EXTI0_1_IRQHandler(void) {
  prv_run_callbacks(0, 2);
}

void EXTI2_3_IRQHandler(void) {
  prv_run_callbacks(2, 4);
}

void EXTI4_15_IRQHandler(void) {
  prv_run_callbacks(4, 16);
}

StatusCode gpio_it_sw_interrupt(GPIOAddress *address) {
  if (address->port >= NUM_GPIO_PORTS || address->pin >= NUM_GPIO_PINS) {
    return status_code(STATUS_CODE_INVALID_ARGS);
  }

  exti_software_interrupt(address->pin);
}
