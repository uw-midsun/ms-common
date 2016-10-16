#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "objpool.h"

#define EVENT_QUEUE_SIZE 20

typedef struct Event {
  ObjectMarker marker;
  uint16_t id;
  uint16_t data;
} Event;

// Initializes the event queue.
void event_queue_init(void);

// Raises an event in the global event queue.
bool event_raise(const Event *e);

// Returns the next event to be processed.
// Note that events are processed by priority.
bool event_process(Event *e);
