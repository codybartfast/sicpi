#ifndef SICPSTD_H
#define SICPSTD_H

#include <inttypes.h>
#include <float.h>
#include <limits.h>
#include <stdlib.h>

typedef int64_t integer;
#define integer_min INT64_MIN
#define integer_max INT64_MAX

typedef double floating;
#define floating_max DBL_MAX
#define floating_min (-floating_max)

typedef uint32_t meta_data;
#define NO_META_DATA 0

void *mallocx(const size_t size, const char *caller);
char *strdupx(const char *s, const char *caller);

#define unused(x) (void)(x)

#endif
