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

bool is_self_evaluating(const object exp);
bool is_variable(const object exp);
object text_of_quotation(const object exp);


object lookup_variable_value(const object var, object env);

object operator(const object exp);
object operands(const object exp);
bool is_no_operands(const object exp);
object first_operand(const object ops);
object rest_operands(const object ops);

//
// §4.1.4 Running the Evaluator as a Program

object the_global_environment(void);
object apply_primitive_procedure(object proc, object args);

#endif
