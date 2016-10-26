// The object pool uses a flag and signature to verify a given node and determine its state.
// We generate the signature from the second byte of the object pool's address, assuming that
// memory addresses are generally aligned and within a certain block of memory and so the first
// and last bytes have a high chance of collision.
#include <string.h>
#include <stdio.h>
#include "objpool.h"

#define OBJPOOL_SIGNATURE(pool) ((uint8_t)((uint8_t *)pool)[1] + 1)

void objpool_init_verbose(ObjectPool *pool, void *nodes, size_t num_nodes,
                          size_t node_size, objpool_node_init_fn init_node) {
  memset(pool, 0, sizeof(*pool));

  pool->nodes = nodes;
  pool->num_nodes = num_nodes;
  pool->node_size = node_size;
  pool->init_node = init_node;

  // Set the node memory block to the object pool signature so we pass the signature check
  memset(pool->nodes, OBJPOOL_SIGNATURE(pool), num_nodes * node_size);

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
  ObjectMarker *marker = node;

  if (node == NULL || marker->signature != OBJPOOL_SIGNATURE(pool)) {
    return;
  }

  memset(node, 0, pool->node_size);
  if (pool->init_node != NULL) {
    pool->init_node(node);
  }

  marker->free = true;
  marker->signature = OBJPOOL_SIGNATURE(pool);
}
