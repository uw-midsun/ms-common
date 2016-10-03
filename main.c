#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "fsm.h"

#define DUMP_STATE() printf("Entered %s\n", __func__)

FSM test_fsm;

typedef enum {
  EVENT_A = 0,
  EVENT_B,
  EVENT_C,
  NUM_EVENTS
} EventID;

FSM_DECLARE_STATE(state_a);
FSM_DECLARE_STATE(state_b);
FSM_DECLARE_STATE(state_c);

int main() {
  srand(time(NULL));
  fsm_init(&test_fsm, "Test FSM", state_a);

  while (true) {
    struct Event e = {
      .id = rand() % NUM_EVENTS
    };

    if (fsm_process_event(&test_fsm, &e)) {
      printf("[%s] Processed event %u, moving %s -> %s\n",
             test_fsm.name, e.id, test_fsm.last_state_name, test_fsm.state_name);
    } else {
      printf("[%s] Failed to process event %u in %s\n",
             test_fsm.name, e.id, test_fsm.state_name);
    }

    sleep(1);
  }

  return 0;
}

FSM_DECLARE_STATE(state_a) {
  DUMP_STATE();
  switch (e->id) {
    case EVENT_B:
      FSM_TRANSITION(state_b);
    case EVENT_C:
      FSM_TRANSITION(state_c);
  }
}

FSM_DECLARE_STATE(state_b) {
  DUMP_STATE();
  switch (e->id) {
    case EVENT_A:
      FSM_TRANSITION(state_a);
  }
}

FSM_DECLARE_STATE(state_c) {
  DUMP_STATE();
  switch(e->id) {
    case EVENT_B:
      FSM_TRANSITION(state_b);
  }
}
