#include "status.h"

#include <string.h>

Status status_create(const StatusCode code, char* message, const int length) {
  if (code >= NUM_STATUS_CODE) {
    return status_msg(STATUS_CODE_INVALID_ARGS, "code invalid");
  } else if (length > STATUS_MESSAGE_SIZE) {
    return status_msg(STATUS_CODE_INVALID_ARGS, "message too long");
  }
  Status status = {.code = code };
  strcpy(status.message, message);
  return status;
}
