#pragma once
// Internal FSM header. Used to clean up the main interface.

#define _FSM_DECLARE_STATE(state) \
_FSM_STATE_TRANSITION(state); \
static State state = { .name = #state, .table = prv_fsm_##state }

#define _FSM_STATE_TRANSITION(state) \
static void prv_fsm_##state(FSM *fsm, const Event *e, bool *transitioned)

#define _FSM_ADD_TRANSITION(event_id, state) \
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

// Initializes an FSM state with an output function.
#define _fsm_state_init(state, output_func) \
do { \
  state.output = (output_func); \
  state.name = #state; \
  state.table = prv_fsm_##state; \
} while (0)
