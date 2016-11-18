#include "debug.h"

#ifndef NDEBUG

void debug_assert(const char* filename, const char* function, unsigned line, const char* expression)
{
    printf("%s:%u %s: Assertion `%s` failed.\n", filename, line, function, expression);
}

#endif
