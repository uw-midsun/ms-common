#pragma once
#include <stdint.h>
#include <stdbool.h>

#define EVENT_QUEUE_SIZE 20

typedef struct Event {
  uint16_t id;
  uint16_t data;
} Event;

// Initializes the event queue.
void event_queue_init(void);

// Raises an event in the global event queue.
bool event_raise(Event *e);

// Returns the next event to be processed.
// Note that events are processed by priority.
Event event_process(void);
