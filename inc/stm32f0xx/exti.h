#pragma once
// Extended interrupt and event controller (EXTI) configuration. This code controls the enabling
// of interrupts, software interrupts and control of pending interrupt bit for each line.
#include <stdint.h>

#include "status.h"

typedef enum {
  EXTI_TYPE_INTERRUPT = 0,
  EXTI_TYPE_EVENT,
  NUM_EXTI_TYPE,
} EXTIType;

typedef enum {
  EXTI_EDGE_RISING = 0,
  EXTI_EDGE_RISING_FALLING,
  EXTI_EDGE_FALLING,
  NUM_EXTI_EDGE,
} EXTIEdge;

typedef enum {
  EXTI_PENDING_RESET = 0,
  EXTI_PENDING_SET,
} EXTIPending;

typedef struct EXTISettings {
  EXTIType type;
  EXTIEdge edge;
} EXTISettings;

// Enable extended interrupt by line number with the specified settings and the provided callback.
StatusCode exti_enable(uint8_t line, EXTISettings* settings);

// TODO(ELEC-38): Assess the need for exti_disable.

// Trigger a SW interrupt on the given line. This is the same as setting the pending interrupt flag
// to SET.
StatusCode exti_software_interrupt(uint8_t line);

// Get the current state for the EXTI pending interrupt flag.
StatusCode exti_get_pending(uint8_t line, EXTIPending* pending_bit);

// Clears the EXTI pending interrupt flag.
StatusCode exti_clear_pending(uint8_t line);
