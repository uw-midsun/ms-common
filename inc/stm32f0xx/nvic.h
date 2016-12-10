#pragma once
// Nested vectored interrupt controller. This enables interrupt priority controls on devices which
// support it. Should always be called from the implementation of a particular set of interrupts as
// opposed to being called directly in a project.
#include <stdint.h>

#include "status.h"

// Specify the interrupt priority and enable the IRQ channel.
StatusCode nvic_enable(uint8_t irq_channel, uint8_t priority);

// TODO(ELEC-38): Determine the need for sleep mode control and disable for NVIC.
