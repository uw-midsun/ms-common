#include "gpio_it.h"

#include <stdint.h>
#include <stdlib.h>

#include "exti.h"
#include "gpio.h"
#include "misc.h"
#include "nvic.h"
#include "status.h"
#include "stm32f0xx.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_syscfg.h"

#define GPIO_NUM_PINS 16
#define GPIO_NUM_PORTS 6
#define GPIO_IT_MAX_PRIORITY 4

static gpio_it_callback s_gpio_it_callbacks[GPIO_NUM_PINS];

StatusCode gpio_it_enable(GPIOAddress *address, GPIOITSettings *settings,
                          gpio_it_callback callback) {
  if (address->port >= GPIO_NUM_PORTS || address->pin >= GPIO_NUM_PINS ||
      settings->edge >= NUM_GPIO_IT_EDGE || settings->type >= NUM_GPIO_IT_TYPE ||
      settings->priority >= GPIO_IT_MAX_PRIORITY) {
    return status_code(STATUS_CODE_INVALID_ARGS);
  }

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  SYSCFG_EXTILineConfig(address->port, address->pin);

  EXTISettings exti_settings = { .type = (EXTIType)settings->type,
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
