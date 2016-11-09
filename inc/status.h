#pragma once
// Status Library for more verbose error handling
#include <string.h>

// StatusCodes for various errors, keep these generic. Never assume their order is fixed so refer to
// them by name only.
typedef enum {
  STATUS_CODE_OK = 0,
  STATUS_CODE_UNKNOWN,
  STATUS_CODE_INVALID_ARGS,
  STATUS_CODE_RESOURCE_EXHAUSTED,
  STATUS_CODE_UNREACHABLE,
  STATUS_CODE_TIMEOUT,
  STATUS_CODE_EMPTY,
  STATUS_CODE_OUT_OF_RANGE,
  STATUS_CODE_UNIMPLEMENTED,
  NUM_STATUS_CODE,
} StatusCode;

#define STATUS_MESSAGE_SIZE 48

typedef struct Status {
  StatusCode code;
  char message[STATUS_MESSAGE_SIZE];
} Status;

// Creates a status struct containing an error code and optionally a message.
Status status_create(const StatusCode code, char* message, const int length);

// A macro for returning with just an error code.
#define status_new(code) status_create(code, "", 0)

// A cleaner way to define a status with a message.
#define status_msg(code, message) status_create(code, message, strlen(message))

// A macro for checking if a status is OK please use this as it is much cleaner and a bit shorter.
#define status_ok(status) status.code == STATUS_CODE_OK

// TODO(ELEC-41): Add support for dumping error STDOUT/STDERR or CAN for debugging.
