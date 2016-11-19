#include "status.h"

#include "misc.h"
#include "unity.h"

void setup_test(void) { }

void teardown_test(void) { }

static StatusCode prv_with_msg() {
  return status_msg(STATUS_CODE_RESOURCE_EXHAUSTED, "my-message");
}

// Verifies most common use cases.
void test_status_create_valid(void) {
  // Most basic use case.
  StatusCode ok = status_code(STATUS_CODE_UNIMPLEMENTED);
  if (!ok) {
    Status status = status_get();
    TEST_ASSERT_EQUAL(STATUS_CODE_UNIMPLEMENTED, status.code);
    TEST_ASSERT_EQUAL_STRING("test_status_create_valid", status.caller);
    TEST_ASSERT_EQUAL_STRING("", status.message);
  }

  // Most basic use case with a message.
  ok = status_msg(STATUS_CODE_UNKNOWN, "error");
  if (!ok) {
    Status status = status_get();
    TEST_ASSERT_EQUAL(STATUS_CODE_UNKNOWN, status.code);
    TEST_ASSERT_EQUAL_STRING("test_status_create_valid", status.caller);
    TEST_ASSERT_EQUAL_STRING("error", status.message);
  }

  // Slightly indirect use case.
  ok = prv_with_msg();
  if (!ok) {
    Status status = status_get();
    TEST_ASSERT_EQUAL(STATUS_CODE_RESOURCE_EXHAUSTED, status.code);
    TEST_ASSERT_EQUAL_STRING("prv_with_msg", status.caller);
    TEST_ASSERT_EQUAL_STRING("my-message", status.message);
  }
}

// Verifies the behavior when a code greater than NUM_STATUS_CODE is entered.
void test_status_create_invalid_code(void) {
  StatusCode statuscode = status_code(NUM_STATUS_CODE + 1);
  Status status = status_get();
  TEST_ASSERT_EQUAL(NUM_STATUS_CODE + 1, status.code);
  TEST_ASSERT_EQUAL_STRING("test_status_create_invalid_code", status.caller);
  TEST_ASSERT_EQUAL_STRING("", status.message);
}

static StatusCode prv_ok_or_return() {
  status_ok_or_return(status_code(STATUS_CODE_OK));
  status_ok_or_return(status_msg(STATUS_CODE_TIMEOUT, "This should work."));
  return status_code(STATUS_CODE_UNKNOWN);
}

void test_status_ok_or_return(void) {
  StatusCode ok = prv_ok_or_return();
  Status status = status_get();
  TEST_ASSERT_EQUAL(STATUS_CODE_TIMEOUT, status.code);
  TEST_ASSERT_EQUAL_STRING("prv_ok_or_return", status.caller);
  TEST_ASSERT_EQUAL_STRING("This should work.", status.message);
  // THIS LINE IS SUPER EASY TO BREAK IF ANYTHING CHANGES ABOVE.
  TEST_ASSERT_EQUAL_STRING(__FILE__ ":" STRINGIFY(55), status.source);
}
