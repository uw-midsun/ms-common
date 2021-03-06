// Implements a min-heap.
// Modified from http://cs.smu.ca/~pawan/teach/csc342/98-99-2/book/chapter9/minheap.h
#include <stdlib.h>
#include <string.h>

#include "pqueue.h"
#include "status.h"

void pqueue_init(PQueue *queue, PQueueNode *nodes, size_t num_nodes) {
  memset(queue, 0, sizeof(*queue));

  queue->nodes = nodes;
  queue->max_nodes = num_nodes - 1;  // 1-indexed heap - throw away one node
}

StatusCode pqueue_push(PQueue *queue, void *data, uint16_t prio) {
  if (queue->size == queue->max_nodes) {
    // Ran out of space.
    return status_code(STATUS_CODE_RESOURCE_EXHAUSTED);
  }

  // Begin at new leaf, bubble up
  size_t i = ++queue->size;
  while (i != 1 && queue->nodes[i / 2].prio > prio) {
    queue->nodes[i] = queue->nodes[i / 2];
    i /= 2;
  }

  queue->nodes[i].prio = prio;
  queue->nodes[i].data = data;

  return STATUS_CODE_OK;
}

void *pqueue_pop(PQueue *queue) {
  if (queue->size == 0) {
    return NULL;
  }

  // Minimum element
  void *data = queue->nodes[1].data;

  // Find place for last element
  size_t last_elem = queue->size--;
  size_t i = 1, child = 2;
  while (child <= queue->size) {
    // Set child to min(left, right)
    if (child < queue->size && queue->nodes[child].prio > queue->nodes[child + 1].prio) {
      child++;
    }

    if (queue->nodes[last_elem].prio <= queue->nodes[child].prio) {
      break;
    }

    // Element does not fit - move child up and go down a level
    queue->nodes[i] = queue->nodes[child];
    i = child;
    child *= 2;
  }

  queue->nodes[i] = queue->nodes[last_elem];

  return data;
}

size_t pqueue_size(PQueue *queue) {
  return queue->size;
}
