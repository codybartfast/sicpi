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

// §4.1.2 Representing Expressions

bool is_self_evaluating(const object obj);
bool is_variable(const object obj);

object lookup_variable_value(const object var, object env);

//
// §4.1.4 Running the Evaluator as a Program

object the_global_environment(void);

#endif
