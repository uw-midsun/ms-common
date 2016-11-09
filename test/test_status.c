#include "status.h"

#include "misc.h"
#include "unity.h"

void setup_test(void) {}

void teardown_test(void) {}

// status_create

// Verifies a few common use cases work.
void test_status_create_valid(void) {
  // Most common use case
  Status status = status_new(STATUS_CODE_UNIMPLEMENTED);
  TEST_ASSERT_EQUAL(STATUS_CODE_UNIMPLEMENTED, status.code);
  TEST_ASSERT_EQUAL_STRING("", status.message);

  // Most common verbose use case.
  status = status_msg(STATUS_CODE_EMPTY, "Nothing here.");
  TEST_ASSERT_EQUAL(STATUS_CODE_EMPTY, status.code);
  TEST_ASSERT_EQUAL_STRING("Nothing here.", status.message);

  // Second most common verbose use case.
  char mystery[] = "It's a mystery";
  status = status_create(STATUS_CODE_UNKNOWN, mystery, SIZEOF_ARRAY(mystery));
  TEST_ASSERT_EQUAL(STATUS_CODE_UNKNOWN, status.code);
  TEST_ASSERT_EQUAL_STRING(mystery, status.message);
}

// Verifies the behavior when invalid args are entered.
void test_status_create_invalid_args(void) {
  Status status =
      status_msg(STATUS_CODE_TIMEOUT, "Took toooooooooooooooooooooooooooooooooooooo long");
  TEST_ASSERT_EQUAL(STATUS_CODE_INVALID_ARGS, status.code);
  TEST_ASSERT_EQUAL_STRING("message too long", status.message);

  status = status_msg(NUM_STATUS_CODE, "message");
  TEST_ASSERT_EQUAL(STATUS_CODE_INVALID_ARGS, status.code);
  TEST_ASSERT_EQUAL_STRING("code invalid", status.message);
}

// Verifies the status ok macro works.
void test_status_ok_macro(void) {
  Status status = status_new(STATUS_CODE_OK);
  TEST_ASSERT_TRUE(status_ok(status));

  status = status_new(STATUS_CODE_TIMEOUT);
  TEST_ASSERT_FALSE(status_ok(status));
}
