#pragma once
// Generic priority queue
#include <stdint.h>
#include <stdbool.h>

typedef struct PQueueNode {
  void *data;
  uint16_t prio;
} PQueueNode;

typedef struct PQueue {
  PQueueNode *nodes;
  size_t max_nodes;
  size_t size;
} PQueue;

void pqueue_init(PQueue *queue, PQueueNode *nodes, size_t max_nodes);

bool pqueue_push(PQueue *queue, void *data, uint16_t prio);

void *pqueue_pop(PQueue *queue);

size_t pqueue_size(PQueue *queue);
