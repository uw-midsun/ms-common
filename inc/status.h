#pragma once
// Status Library for more verbose error handling
#include <stdbool.h>
#include <stdint.h>

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

typedef struct Status {
  StatusCode code;
  const char* caller;
  const char* message;
} Status;

// Creates a status struct containing an error code and optionally a message. This should only be
// called via the macros.
bool prv_status_create(const StatusCode code, const char* caller, const char* message);

// Get a copy of the global status so it can be used safely.
Status status_get();

// Macros for convenience.
#define status_new(code) prv_status_create(code, __FUNCTION__, "")
#define status_msg(code, message) prv_status_create(code, __FUNCTION__, message)

// Use to forward failures or continue on success.
#define status_ok_or_return(ok) \
  if (!ok) return false
