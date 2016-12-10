#pragma once

#include "status.h"

typedef void (*interrupt_callback)(void);

typedef enum {
  INTERRUPT_TYPE_INTERRUPT = 0,
  INTERRUPT_TYPE_EVENT,
  NUM_INTERRUPT_TYPE,
} InterruptType;

typedef enum {
  INTERRUPT_EDGE_RISING = 0,
  INTERRUPT_EDGE_RISING_FALLING,
  INTERRUPT_EDGE_FALLING,
  NUM_INTERRUPT_EDGE,
} InterruptEdge;

typedef enum {
  INTERRUPT_PRIORITY_HIGH = 0,
  INTERRUPT_PRIORITY_NORMAL,
  INTERRUPT_PRIORITY_LOW,
  NUM_INTERRUPT_PRIORITY,
} InterruptPriority;

typedef struct InterruptSettings {
  InterruptType type;
  InterruptEdge edge;
  InterruptPriority priority;
} InterruptSettings;
