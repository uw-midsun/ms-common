#include <string.h>
#include <stdio.h>
#include "objpool.h"

void objpool_init(ObjectPool *pool, void *nodes, size_t num_nodes, size_t node_size,
                  objpool_node_init_fn init_node) {
  memset(pool, 0, sizeof(*pool));

  pool->nodes = nodes;
  pool->num_nodes = num_nodes;
  pool->node_size = node_size;
  pool->init_node = init_node;

  for (size_t i = 0; i < num_nodes; i++) {
    void *node = (void *)((uintptr_t)pool->nodes + (i * pool->node_size));
    objpool_free_node(pool, node);
  }
}

void *objpool_get_node(ObjectPool *pool) {
  for (size_t i = 0; i < pool->num_nodes; i++) {
    ObjectMarker *node = (ObjectMarker *)((uintptr_t)pool->nodes + (i * pool->node_size));
    if (node->free) {
      node->free = false;
      return node;
    }
  }

  return NULL;
}

void objpool_free_node(ObjectPool *pool, void *node) {
  pool->init_node(node);

  ObjectMarker *marker = node;
  marker->free = true;
}
