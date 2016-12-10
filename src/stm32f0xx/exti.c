#include "stm32f0xx/exti.h"

#include <stdint.h>

#include "status.h"
#include "stm32f0xx_exti.h"

#define MAX_LINE 31

StatusCode exti_enable(uint8_t line, EXTISettings* settings) {
  if (line > MAX_LINE || settings->type >= NUM_EXTI_TYPE || settings->edge >= NUM_EXTI_EDGE) {
    return status_code(STATUS_CODE_INVALID_ARGS);
  }

  // Note the way mode and trigger are set here is somewhat dangerous.
  EXTI_InitTypeDef init_struct = {.EXTI_Line = 0x01 << line,
                                  .EXTI_Mode = 0x04 * settings->type,
                                  .EXTI_Trigger = 0x08 + 0x04 * settings->edge,
                                  .EXTI_LineCmd = ENABLE };
  EXTI_Init(&init_struct);
  return STATUS_CODE_OK;
}

StatusCode exti_software_interrupt(uint8_t line) {
  if (line == 18 || line > 22) {
    return status_code(STATUS_CODE_INVALID_ARGS);
  }

  EXTI_GenerateSWInterrupt(0x01 << line);
  return STATUS_CODE_OK;
}

StatusCode exti_get_pending(uint8_t line, EXTIPending* pending_bit) {
  if (line > MAX_LINE) {
    return status_code(STATUS_CODE_INVALID_ARGS);
  }

  *pending_bit = (EXTIPending)EXTI_GetITStatus(0x01 << line);
  return STATUS_CODE_OK;
}

StatusCode exti_clear_pending(uint8_t line) {
  if (line > MAX_LINE) {
    return status_code(STATUS_CODE_INVALID_ARGS);
  }

  EXTI_ClearITPendingBit(0x01 << line);
  return STATUS_CODE_OK;
}
