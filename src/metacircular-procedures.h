/*
 * Procudures from the Metacircular Evaluator
 * 	(https://www.sicp-book.com/book-Z-H-26.html#%_sec_4.1)
 *
 * The metacircular evaluator is not implemented here, but many of it's helper
 * procedures are used by the explicit control evaluator (on which this
 * interpreter is based) and so those helpers are recreated here.
 */

#ifndef H_METACIRCULAR_PROCEDURES
#define H_METACIRCULAR_PROCEDURES

#include "object.h"

#include <stdbool.h>

bool is_self_evaluating(object obj);

#endif
