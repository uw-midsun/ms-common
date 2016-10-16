#include <stdlib.h>
#include <string.h>
#include "pqueue.h"

void pqueue_init(PQueue *queue, PQueueNode *nodes, size_t max_nodes) {
  memset(queue, 0, sizeof(*queue));

  queue->nodes = nodes;
  queue->max_nodes = max_nodes;
}

bool pqueue_push(PQueue *queue, void *data, uint16_t prio) {
  if (queue->size + 1 >= queue->max_nodes) {
    // Ran out of space - TODO: handle errors better
    return false;
  }

  size_t i = queue->size + 1;
  size_t j = i / 2;
  while (i > 1 && queue->nodes[j].prio > prio) {
    queue->nodes[i] = queue->nodes[j];
    i = j;
    j /= 2;
  }

  queue->nodes[i].prio = prio;
  queue->nodes[i].data = data;
  queue->size++;
}

void *pqueue_pop(PQueue *queue) {
  if (queue->size == 0) {
    return NULL;
  }

  void *data = queue->nodes[1].data;
  queue->nodes[1] = queue->nodes[queue->size];
  queue->size--;

  size_t i = 1, j = 0, k = 0;
  while (true) {
    k = i;
    j = 2 * i;
    if (j <= queue->size && queue->nodes[j].prio < queue->nodes[k].prio) {
      k = j;
    }
    if (j + 1 <= queue->size && queue->nodes[j + 1].prio < queue->nodes[k].prio) {
      k = j + 1;
    }
    if (k == i) {
      break;
    }
    queue->nodes[i] = queue->nodes[k];
    i = k;
  }
  queue->nodes[i] = queue->nodes[queue->size + 1];

  return data;
}

size_t pqueue_size(PQueue *queue) {
  return queue->size;
}
