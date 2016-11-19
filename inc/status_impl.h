#pragma once

#include <stdbool.h>

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

// Creates a status struct containing an error code and optionally a message. This should only be
// called via the macros.
bool status_impl_create(const StatusCode code, const char* caller, const char* message);
