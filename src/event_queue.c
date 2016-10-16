#include "event_queue.h"
#include "pqueue.h"
#include "objpool.h"

typedef struct EventQueue {
  ObjectPool pool;
  PQueue queue;
  PQueueNode queue_nodes[EVENT_QUEUE_SIZE];
  Event event_nodes[EVENT_QUEUE_SIZE];
} EventQueue;

static EventQueue queue;

static prv_init_node(void *node) {
  Event *e = node;
  memset(e, 0xA5, sizeof(*e));
}

void event_queue_init(void) {
  pqueue_init(&queue.queue, queue.queue_nodes, EVENT_QUEUE_SIZE);
  objpool_init(&queue.pool, queue.event_nodes, EVENT_QUEUE_SIZE,
               sizeof(queue.event_nodes[0]), prv_init_node);
}

bool event_raise(const Event *e) {
  Event *node = objpool_get_node(&queue.pool);
  if (node == NULL) {
    return false;
  }

  *node = *e;
  pqueue_push(&queue.queue, node, node->id);

  return true;
}

bool event_process(Event *e) {
  Event *node = pqueue_pop(&queue.queue);
  if (node == NULL) {
    return false;
  }

  *e = *node;
  objpool_free_node(&queue.pool, node);

  return true;
}
