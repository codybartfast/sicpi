#ifndef OBJECT_H
#define OBJECT_H

#include "sicpstd.h"

#include <stdbool.h>
#include <inttypes.h>

typedef union value_union {
	integer integer;
} value_union;

typedef struct object {
	uint8_t value_kind;
	meta_data meta_data;
	value_union value;
} *object;

// General
// =============================================================================
//

meta_data object_meta_data(object obj);

// Number
// =============================================================================
//

bool is_number(object obj);
object from_integer(integer integer, meta_data meta_data);
integer to_integer(object obj);

#endif
