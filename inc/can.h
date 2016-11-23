#pragma once
#include <stdint.h>
#include <stdbool.h>

#include "can_queue.h"

#define CAN_DEVID_MASK (0x1F)
#define CAN_MSGID_MASK (0x3F << 5)

typedef struct CANConfig {
  CANQueue tx_queue;
  CANQueue rx_queue;
  uint16_t speed; // In kbps
  uint8_t num_filters;
} CANConfig;

void can_init(CANConfig *can);

// Returns whether the filter was added properly
bool can_add_filter(CANConfig *can, uint16_t filter, uint16_t mask);

// Returns whether the message was placed in the TX queue
bool can_transmit(CANConfig *can, const CANMessage *msg);

// Returns whether msg was populated with a received CAN message
bool can_receive(CANConfig *can, CANMessage *msg);
