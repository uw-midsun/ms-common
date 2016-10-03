#pragma once
#include <stdint.h>
#include <event_queue.h>

#define FSM_DECLARE_STATE(state) static void state(FSM *fsm, const Event *e, bool *transitioned)
#define FSM_TRANSITION(state) do { \
  fsm->current_state = state; \
  fsm->state_name = #state; \
  fsm->last_state_name = __func__; \
  *transitioned = true; \
  return; \
} while (0)

struct FSM;
typedef void (*State)(struct FSM *fsm, const Event *e, bool *transitioned);

typedef struct FSM {
  const char *name;
  const char *state_name;
  const char *last_state_name;
  State current_state;
} FSM;

// Initializes the FSM.
void fsm_init(FSM *fsm, const char *name, State default_state);

// Returns whether a transition occurred in the FSM.
bool fsm_process_event(FSM *fsm, const Event *e);
