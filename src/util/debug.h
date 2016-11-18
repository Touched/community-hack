#ifndef DEBUG_H
#define DEBUG_H

int printf(const char *format, ...);
int sprintf(char *out, const char *format, ...);
void debug_assert(const char* filename, const char* function, unsigned line, const char* expression);

#ifdef NDEBUG
#define assert(cond) ((void)0)
#else
#define assert(cond) ((cond) ? (void)0 : debug_assert(__FILE__, __FUNCTION__, __LINE__, #cond))
#endif

#endif /* DEBUG_H */
