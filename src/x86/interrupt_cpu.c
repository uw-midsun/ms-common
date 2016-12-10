#include "x86/interrupt_cpu.h"

#include <signal.h>
#include <stdint.h>
#include <unistd.h>

#include "interrupt.h"
#include "status.h"

#define INTERRUPT_CPU_MAX_CALLBACK 255

static uint8_t s_next_id = 0;

static uint8_t s_priority_map[INTERRUPT_CPU_MAX_CALLBACK];
static interrupt_callback s_callbacks[INTERRUPT_CPU_MAX_CALLBACK];

void prv_sig_handler(int signum, siginfo_t *info, void *ptr) {
  if (info->si_value.sival_ptr == NULL) {
    if (info->si_value.sival_int < INTERRUPT_CPU_MAX_CALLBACK) {
      s_callbacks[info->si_value.sival_int]();
    }
  }
}

void interrupt_cpu_init() {
  struct sigaction act;

  sigset_t block_mask;
  sigemptyset(&block_mask);
  sigaddset(&block_mask, SIGRTMIN + INTERRUPT_PRIORITY_LOW);

  act.sa_sigaction = prv_sig_handler;
  act.sa_flags = SA_SIGINFO;
  act.sa_mask = block_mask;
  sigaction(SIGRTMIN + INTERRUPT_PRIORITY_LOW, &act, NULL);

  sigaddset(&block_mask, SIGRTMIN + INTERRUPT_PRIORITY_NORMAL);
  act.sa_mask = block_mask;
  sigaction(SIGRTMIN + INTERRUPT_PRIORITY_NORMAL, &act, NULL);

  sigaddset(&block_mask, SIGRTMIN + INTERRUPT_PRIORITY_HIGH);
  act.sa_mask = block_mask;
  sigaction(SIGRTMIN + INTERRUPT_PRIORITY_HIGH, &act, NULL);
}

StatusCode interrupt_cpu_register(InterruptSettings *settings, interrupt_callback callback,
                                  uint8_t *id) {
  if (s_next_id >= INTERRUPT_CPU_MAX_CALLBACK) {
    return status_msg(STATUS_CODE_RESOURCE_EXHAUSTED, "Too many interrupts registered.");
  } else if (settings->type >= NUM_INTERRUPT_TYPE || settings->edge >= NUM_INTERRUPT_EDGE ||
             settings->priority >= NUM_INTERRUPT_PRIORITY) {
    return status_code(STATUS_CODE_INVALID_ARGS);
  }

  s_priority_map[s_next_id] = settings->priority;
  s_callbacks[s_next_id] = callback;
  *id = s_next_id++;

  return STATUS_CODE_OK;
}

void interrupt_cpu_raise(uint8_t id) {
  siginfo_t value_store;
  value_store.si_value.sival_int = id;
  sigqueue(getpid(), SIGRTMIN + s_priority_map[id], value_store.si_value);
}
