#include "fsm.h"
#include <stdbool.h>

void fsm_init(FSM *fsm, const char *name, State *default_state) {
  //ASSERT(fsm != NULL && name != NULL && default_state != NULL);

  fsm->name = name;
  fsm->current_state = default_state;
}

bool fsm_process_event(FSM *fsm, const Event *e) {
  bool transitioned = false;

  fsm->current_state->table(fsm, e, &transitioned);

  return transitioned;
}
