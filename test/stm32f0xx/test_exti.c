// This covers testing both external interrupts and the functionality of NVIC.

#include <stdint.h>

#include "extra_unity.h"
#include "status.h"
#include "stm32f0xx/exti.h"
#include "stm32f0xx/nvic.h"
#include "unity.h"

void setup_test(void) {
  // Enable nested interrupts on EXTI0_1.
  nvic_enable(5, 2);
}

void teardown_test(void) {}

#define VALID_LINE 1
#define INVALID_LINE 100

// exti_enable

// Test an invalid enable.
void test_exti_enable_invalid(void) {
  EXTISettings settings = {.type = EXTI_TYPE_INTERRUPT, .edge = EXTI_EDGE_RISING };
  TEST_ASSERT_EQUAL(STATUS_CODE_INVALID_ARGS, exti_enable(INVALID_LINE, &settings));
  settings.type = NUM_EXTI_TYPE;
  TEST_ASSERT_EQUAL(STATUS_CODE_INVALID_ARGS, exti_enable(VALID_LINE, &settings));
  settings.type = EXTI_TYPE_INTERRUPT;
  settings.edge = NUM_EXTI_EDGE;
  TEST_ASSERT_EQUAL(STATUS_CODE_INVALID_ARGS, exti_enable(VALID_LINE, &settings));
}

// exti_software_interrupt

// Test an invalid SW interrupt.
void test_exti_software_interrupt_invalid(void) {
  TEST_ASSERT_EQUAL(STATUS_CODE_INVALID_ARGS, exti_software_interrupt(INVALID_LINE));
}

// exti_get_pending

// Test an invalid get.
void test_exti_get_pending_invalid(void) {
  EXTIPending pending_bit;
  TEST_ASSERT_EQUAL(STATUS_CODE_INVALID_ARGS, exti_get_pending(INVALID_LINE, &pending_bit));
}

// exti_clear_pending

// Test an invalid line reset.
void test_exti_clear_pending_invalid(void) {
  TEST_ASSERT_EQUAL(STATUS_CODE_INVALID_ARGS, exti_clear_pending(INVALID_LINE));
}

// exti functional e2e test

// A test of the behavior of all the exti components
void test_exti_e2e(void) {
  EXTISettings settings = {.type = EXTI_TYPE_INTERRUPT, .edge = EXTI_EDGE_RISING };
  TEST_ASSERT_OK(exti_enable(VALID_LINE, &settings));
  EXTIPending pending_bit;
  TEST_ASSERT_OK(exti_get_pending(VALID_LINE, &pending_bit));
  TEST_ASSERT_EQUAL(EXTI_PENDING_RESET, pending_bit);
  TEST_ASSERT_OK(exti_software_interrupt(VALID_LINE));
  TEST_ASSERT_OK(exti_get_pending(VALID_LINE, &pending_bit));
  TEST_ASSERT_EQUAL(EXTI_PENDING_SET, pending_bit);
  TEST_ASSERT_OK(exti_clear_pending(VALID_LINE));
  TEST_ASSERT_OK(exti_get_pending(VALID_LINE, &pending_bit));
  TEST_ASSERT_EQUAL(EXTI_PENDING_RESET, pending_bit);
}
