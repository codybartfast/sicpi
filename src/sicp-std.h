#ifndef SICPSTD_H
#define SICPSTD_H

#include <inttypes.h>
#include <float.h>
#include <limits.h>

typedef int64_t integer;
#define integer_min INT64_MIN
#define integer_max INT64_MAX

typedef double floating;
#define floating_min DBL_MIN
#define floating_max DBL_MAX

typedef uint32_t meta_data;
#define NO_META_DATA 0

#endif
