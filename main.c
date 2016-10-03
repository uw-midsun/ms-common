#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "fsm.h"

typedef enum {
  EVENT_A = 0,
  EVENT_B,
  EVENT_C,
  NUM_EVENTS
} EventID;

FSM test_fsm;
FSM_DECLARE_STATE(state_a);
FSM_DECLARE_STATE(state_b);
FSM_DECLARE_STATE(state_c);

static void prv_print_state(FSM *fsm, const Event *e) {
  printf("Entered %s (event %u occurred)\n", fsm->current_state->name, e->id);
}

int main() {
  srand(time(NULL));

  fsm_state_init(state_a, prv_print_state);
  fsm_state_init(state_b, prv_print_state);
  fsm_state_init(state_c, prv_print_state);
  fsm_init(&test_fsm, "Test FSM", &state_a);

  while (true) {
    Event e = {
      .id = rand() % NUM_EVENTS
    };

    if (fsm_process_event(&test_fsm, &e)) {
      printf("[%s] Processed event %u, moving %s -> %s\n",
             test_fsm.name, e.id, test_fsm.last_state->name, test_fsm.current_state->name);
    } else {
      printf("[%s] Failed to process event %u in %s\n",
             test_fsm.name, e.id, test_fsm.current_state->name);
    }

    sleep(1);
  }

  return 0;
}

FSM_STATE_TRANSITION(state_a) {
  FSM_ADD_TRANSITION(EVENT_B, state_b);
  FSM_ADD_TRANSITION(EVENT_C, state_c);
}

FSM_STATE_TRANSITION(state_b) {
  FSM_ADD_TRANSITION(EVENT_C, state_c);
}

FSM_STATE_TRANSITION(state_c) {
  FSM_ADD_TRANSITION(EVENT_B, state_b);
}
