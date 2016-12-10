// This tests the guard clauses of NVIC the functional test needs EXTI so is part of that test.

#include "extra_unity.h"
#include "interrupt.h"
#include "status.h"
#include "stm32f0xx/nvic.h"
#include "unity.h"

void setup_test(void) {}

void teardown_test(void) {}

#define VALID_IRQ 1
#define INVALID_IRQ 100

// nvic_enable

// Test a valid enable.
void test_nvic_enable_valid(void) {
  TEST_ASSERT_OK(nvic_enable(VALID_IRQ, INTERRUPT_PRIORITY_NORMAL));
}

// Test an invalid enable.
void test_nvic_enable_invalid(void) {
  TEST_ASSERT_EQUAL(STATUS_CODE_INVALID_ARGS, nvic_enable(INVALID_IRQ, INTERRUPT_PRIORITY_NORMAL));
  TEST_ASSERT_EQUAL(STATUS_CODE_INVALID_ARGS, nvic_enable(VALID_IRQ, NUM_INTERRUPT_PRIORITY));
}
