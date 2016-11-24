#include "nvic.h"

#include <stdint.h>

#include "status.h"
#include "stm32f0xx_misc.h"

StatusCode nvic_enable(uint8_t irq_channel, uint8_t priority) {
  if (irq_channel > 31 || priority > 3) {
    return status_code(STATUS_CODE_INVALID_ARGS);
  }

  NVIC_InitTypeDef init_struct = {.NVIC_IRQChannel = irq_channel,
                                  .NVIC_IRQChannelPriority = priority,
                                  .NVIC_IRQChannelCmd = ENABLE };

  NVIC_Init(&init_struct);

  return STATUS_CODE_OK;
}
