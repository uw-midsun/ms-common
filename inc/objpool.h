#pragma once
// Object pool helper
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

// Function to initialize nodes with
typedef void (*objpool_node_init_fn)(void *node);

// All nodes compatible with object pools should begin with an object marker
typedef struct ObjectMarker {
  bool free;
} ObjectMarker;

typedef struct ObjectPool {
  void *nodes;
  size_t num_nodes;
  size_t node_size;
  objpool_node_init_fn init_node;
} ObjectPool;

void objpool_init(ObjectPool *pool, void *nodes, size_t num_nodes, size_t node_size,
                  objpool_node_init_fn init_node);

void *objpool_get_node(ObjectPool *pool);

void objpool_free_node(ObjectPool *pool, void *node);
