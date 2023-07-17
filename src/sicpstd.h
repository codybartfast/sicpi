#ifndef SICPSTD_H
#define SICPSTD_H

#include <inttypes.h>

typedef int64_t integer;
typedef double floating;
typedef uint32_t meta_data;
#define NO_META_DATA = 0;

void alloc_error(char *caller);
void eprintf(const char *msg, ...);
void eprintfx(const char *msg, ...);
void inyim(const char *msg, ...);

#endif
