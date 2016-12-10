#pragma once

#include <stdint.h>

#include "interrupt.h"
#include "status.h"

void interrupt_cpu_init();

StatusCode interrupt_cpu_register(InterruptSettings *settings, interrupt_callback callback,
                                  uint8_t *id);

void interrupt_cpu_raise(uint8_t id);
