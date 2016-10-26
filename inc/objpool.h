#pragma once
// Object Pool Interface
// Manages a pre-allocated array of objects. We use this instead of a heap so we don't need to deal
// with memory fragmentation.
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "misc.h"

// Function to initialize nodes with
typedef void (*objpool_node_init_fn)(void *node);

// All nodes compatible with object pools should begin with an object marker
typedef struct ObjectMarker {
  uint8_t signature;
  bool free;
} ObjectMarker;

typedef struct ObjectPool {
  void *nodes;
  size_t num_nodes;
  size_t node_size;
  objpool_node_init_fn init_node;
} ObjectPool;

// Initializes an object pool given a local array (i.e. not a pointer)
#define objpool_init(pool, nodes, init_fn) \
  objpool_init_verbose(pool, nodes, SIZEOF_ARRAY(nodes), sizeof(nodes[0]), init_fn)

void objpool_init_verbose(ObjectPool *pool, void *nodes, size_t num_nodes,
                          size_t node_size, objpool_node_init_fn init_node);

// Returns the pointer to an object from the pool.
void *objpool_get_node(ObjectPool *pool);

// Releases the specified node
void objpool_free_node(ObjectPool *pool, void *node);
