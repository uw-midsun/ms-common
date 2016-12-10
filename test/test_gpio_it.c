#include <stdbool.h>

#include "extra_unity.h"
#include "gpio_it.h"
#include "interrupt.h"
#include "status.h"
#include "unity.h"

#define GPIO_VALID_PORT 0
#define GPIO_VALID_PIN 0
#define GPIO_INVALID_PORT 100
#define GPIO_INVALID_PIN 100

static bool s_callback_ran;

static GPIOAddress s_valid_address = { GPIO_VALID_PORT, GPIO_VALID_PIN };

static InterruptSettings s_default_settings = { INTERRUPT_TYPE_INTERRUPT, INTERRUPT_EDGE_RISING,
                                                INTERRUPT_PRIORITY_NORMAL };

static void prv_test_callback() {
  s_callback_ran = true;
}

void setup_test(void) {
  s_callback_ran = false;
}

void teardown_test(void) {}

void test_gpio_it_invalid_cases(void) {
  GPIOAddress invalid_address = { GPIO_INVALID_PORT, GPIO_VALID_PIN };
  InterruptSettings invalid_settings = { NUM_INTERRUPT_TYPE, INTERRUPT_EDGE_RISING,
                                         INTERRUPT_PRIORITY_NORMAL };
  TEST_ASSERT_EQUAL(STATUS_CODE_INVALID_ARGS,
                    gpio_it_enable(&invalid_address, &s_default_settings, prv_test_callback));
  TEST_ASSERT_EQUAL(STATUS_CODE_INVALID_ARGS, gpio_it_sw_interrupt(&invalid_address));
  invalid_address.port = GPIO_VALID_PORT;
  invalid_address.port = GPIO_INVALID_PIN;
  TEST_ASSERT_EQUAL(STATUS_CODE_INVALID_ARGS,
                    gpio_it_enable(&invalid_address, &s_default_settings, prv_test_callback));
  TEST_ASSERT_EQUAL(STATUS_CODE_INVALID_ARGS, gpio_it_sw_interrupt(&invalid_address));

  TEST_ASSERT_EQUAL(STATUS_CODE_INVALID_ARGS,
                    gpio_it_enable(&s_valid_address, &invalid_settings, prv_test_callback));
  invalid_settings.type = INTERRUPT_TYPE_INTERRUPT;
  invalid_settings.edge = NUM_INTERRUPT_EDGE;
  TEST_ASSERT_EQUAL(STATUS_CODE_INVALID_ARGS,
                    gpio_it_enable(&s_valid_address, &invalid_settings, prv_test_callback));
  invalid_settings.edge = INTERRUPT_EDGE_RISING;
  invalid_settings.priority = NUM_INTERRUPT_PRIORITY;
}

void test_gpio_it_sw_interrupt(void) {
  TEST_ASSERT_OK(gpio_it_enable(&s_valid_address, &s_default_settings, prv_test_callback));
  TEST_ASSERT_OK(gpio_it_sw_interrupt(&s_valid_address));
  TEST_ASSERT_TRUE(s_callback_ran);
}

void test_gpio_it_nvic(void) {}
