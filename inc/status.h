#pragma once
// Status Library for more verbose error handling

#include "misc.h"

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
  const char* source;
  const char* caller;
  const char* message;
} Status;

// Updates a status struct containing an error code and optionally a message. This should only be
// called via the macros.
StatusCode status_impl_update(const StatusCode code, const char* source, const char* caller,
                              const char* message);

// Get a copy of the global status so it can be used safely.
Status status_get();

// Clears the global status.
void status_clear();

// Macros for convenience.
#define status_code(code) \
  status_impl_update((code), (__FILE__ ":" STRINGIFY(__LINE__)), (__FUNCTION__), (""))
#define status_msg(code, message) \
  status_impl_update((code), (__FILE__ ":" STRINGIFY(__LINE__)), (__FUNCTION__), (message))
#define status_ok(code) (STATUS_CODE_OK == (code))

// Use to forward failures or continue on success.
#define status_ok_or_return(code) \
  if (code) return code
