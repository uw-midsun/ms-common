#include "status.h"

#include <stdbool.h>
#include <stdlib.h>

static Status global_status = {.code = STATUS_CODE_OK, .caller = NULL, .message = "" };

bool prv_status_create(const StatusCode code, const char* caller, const char* message) {
  if (code >= NUM_STATUS_CODE) {
    global_status.code = NUM_STATUS_CODE;
  } else {
    global_status.code = code;
  }
  global_status.caller = caller;
  global_status.message = message;
  if (code == STATUS_CODE_OK) {
    return true;
  }
  return false;
}

Status status_get() {
  return global_status;
}

bool status_ok() {
  return global_status.code == STATUS_CODE_OK;
}
