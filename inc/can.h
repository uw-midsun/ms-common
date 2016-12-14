#pragma once
#include <stdint.h>
#include <stdbool.h>

#include "can_queue.h"
#include "event_queue.h"

typedef struct CANConfig {
  CANQueue tx_queue;
  CANQueue rx_queue;
  EventID event_id;
  uint16_t speed; // In kbps
  uint8_t num_filters;
} CANConfig;

void can_init(CANConfig *can);

// Returns whether the filter was added properly
StatusCode can_add_filter(CANConfig *can, uint16_t filter, uint16_t mask);

// Returns whether the message was placed in the TX queue
StatusCode can_transmit(CANConfig *can, const CANMessage *msg);

// Returns whether msg was populated with a received CAN message
StatusCode can_receive(CANConfig *can, CANMessage *msg);
