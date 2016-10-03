#pragma once
#include <stdint.h>
#include <event_queue.h>

#define FSM_DECLARE_STATE(state) \
FSM_STATE_TRANSITION(state); \
static State state = { .name = #state, .table = prv_fsm_##state }

#define FSM_STATE_TRANSITION(state) \
static void prv_fsm_##state(FSM *fsm, const Event *e, bool *transitioned)

#define FSM_ADD_TRANSITION(event_id, state) \
do { \
  if (e->id == event_id) { \
    fsm->last_state = fsm->current_state; \
    fsm->current_state = &state; \
    *transitioned = true; \
\
    if (fsm->current_state->output != NULL) { \
      fsm->current_state->output(fsm, e); \
    } \
\
    return; \
  } \
} while (0)

#define fsm_state_init(state, output_func) \
do { \
  state.output = (output_func); \
  state.name = #state; \
  state.table = prv_fsm_##state; \
} while (0)

struct FSM;
typedef void (*StateOutput)(struct FSM *fsm, const Event *e);
typedef void (*StateTransition)(struct FSM *fsm, const Event *e, bool *transitioned);

typedef struct State {
  const char *name;
  StateOutput output;
  StateTransition table;
} State;

typedef struct FSM {
  const char *name;
  State *last_state;
  State *current_state;
} FSM;

// Initializes the FSM.
void fsm_init(FSM *fsm, const char *name, State *default_state);

// Returns whether a transition occurred in the FSM.
bool fsm_process_event(FSM *fsm, const Event *e);
