#pragma once
// Wrapper for a backed priority queue specifically for CAN messages
#include "pqueue_backed.h"

#define CAN_QUEUE_SIZE 10

// 11 bits: 6 -> message ID, 5 -> device ID
typedef struct CANMessage {
  union {
    uint16_t id;
    struct {
      uint16_t dev_id:5;
      uint16_t msg_id:6;
    };
  };
  uint8_t dlc;
  bool rtr;
  union {
    uint64_t data;
    uint32_t data_u32[2];
    uint16_t data_u16[4];
    uint8_t data_u8[8];
  };
} CANMessage;

typedef struct CANQueue {
  PQueueBacked pqueue;
  PQueueNode queue_nodes[CAN_QUEUE_SIZE + 1];
  CANMessage msg_nodes[CAN_QUEUE_SIZE];
} CANQueue;

void can_queue_init(CANQueue *queue);

bool can_queue_push(CANQueue *queue, const CANMessage *msg);

bool can_queue_pop(CANQueue *queue, CANMessage *msg);

size_t can_queue_size(CANQueue *queue);
