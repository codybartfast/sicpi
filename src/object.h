#ifndef OBJECT_H
#define OBJECT_H

#include "sicp-std.h"

#include <stdbool.h>
#include <inttypes.h>

typedef union value_union {
	const floating floating;
	const integer integer;
} value_union;

typedef struct object {
	const uint8_t value_kind;
	const meta_data meta_data;
	const value_union value;
} *object;

//
// General
// =============================================================================
//

meta_data object_meta_data(object obj);
void object_free(object obj);

//
// Number
// =============================================================================
//

bool is_number(object obj);
object from_integer(integer integer, meta_data meta_data);
integer to_integer(object obj);
object from_floating(floating floating, meta_data meta_data);
floating to_floating(object obj);

#endif
