#include "status.h"

static Status s_global_status = {.source = "", .caller = "", .message = "" };

StatusCode status_impl_update(const StatusCode code, const char* source, const char* caller,
                              const char* message) {
  s_global_status.code = code;
  s_global_status.source = source;
  s_global_status.caller = caller;
  s_global_status.message = message;
  return code;
}

Status status_get() {
  return s_global_status;
}
