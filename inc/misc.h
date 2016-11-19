#pragma once
// Common helper macros

#define SIZEOF_ARRAY(arr) (sizeof((arr)) / sizeof((arr)[0]))

#define STRINGANIZE_(x) #x
#define STRINGANIZE(x) STRINGANIZE_(x)
