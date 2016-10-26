#include "objpool.h"
#include "unity.h"

#define TEST_OBJPOOL_SIZE 15
#define TEST_OBJPOOL_DEFAULT UINT16_MAX

typedef struct TestObject {
  ObjectMarker marker;
  uint16_t data;
} TestObject;

static ObjectPool gv_pool;
static TestObject gv_nodes[TEST_OBJPOOL_SIZE];

static void prv_node_init(void *node) {
  TestObject *obj = node;
  obj->data = TEST_OBJPOOL_DEFAULT;
}

void setup_test(void) {
  objpool_init(&gv_pool, gv_nodes, prv_node_init);
}

void teardown_test(void) { }

void test_objpool_multi(void) {
  TestObject *nodes[TEST_OBJPOOL_SIZE] = { 0 };

  for (int i = 0; i < TEST_OBJPOOL_SIZE; i++) {
    nodes[i] = objpool_get_node(&gv_pool);
    nodes[i]->data = i;
  }

  for (int i = 0; i < TEST_OBJPOOL_SIZE; i++) {
    TEST_ASSERT_EQUAL(i, nodes[i]->data);
  }

  for (int i = 0; i < TEST_OBJPOOL_SIZE; i++) {
    objpool_free_node(&gv_pool, nodes[i]);
  }

  for (int i = 0; i < TEST_OBJPOOL_SIZE; i++) {
    TestObject *node = objpool_get_node(&gv_pool);

    // Expect our nodes to be reset
    TEST_ASSERT_EQUAL(TEST_OBJPOOL_DEFAULT, node->data);
  }
}

void test_objpool_too_many(void) {
  TestObject *nodes[TEST_OBJPOOL_SIZE] = { 0 };

  for (int i = 0; i < TEST_OBJPOOL_SIZE; i++) {
    nodes[i] = objpool_get_node(&gv_pool);
    nodes[i]->data = i;
  }

  TEST_ASSERT_NULL(objpool_get_node(&gv_pool));

  objpool_free_node(&gv_pool, nodes[TEST_OBJPOOL_SIZE - 1]);

  TEST_ASSERT_NOT_NULL(objpool_get_node(&gv_pool));
}

void test_objpool_invalid_free(void) {
  // Expect this not to segfault
  objpool_free_node(&gv_pool, NULL);

  // Purposely access out of bounds memory
  objpool_free_node(&gv_pool, &gv_nodes[TEST_OBJPOOL_SIZE]);
}

void test_objpool_free_other_pool(void) {
  ObjectPool pool;
  TestObject nodes[1];

  objpool_init(&pool, nodes, NULL);

  TestObject *node = objpool_get_node(&pool);
  node->data = 0x1234;
  TEST_ASSERT_NOT_NULL(node);

  // Invalid operation - should fail silently
  objpool_free_node(&gv_pool, node);

  TEST_ASSERT_EQUAL(0x1234, node->data);
}
