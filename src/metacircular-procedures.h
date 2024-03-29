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

//
// §4.1.2 Representing Expressions
//

bool is_self_evaluating(const object exp);

bool is_variable(const object exp);

bool is_quoted(const object exp);
object text_of_quotation(const object exp);

// assign

bool is_definition(const object exp);
object definition_variable(object exp);
object definition_value(object exp);

bool is_lambda(const object exp);
object lambda_parameters(const object exp);
object lambda_body(const object exp);
object make_lambda(const object parameters, const object body);

bool is_if(const object exp);
object if_predicate(object exp);
object if_consequent(object exp);
object if_alternative(object exp);

object make_begin(object exp);
object cond_to_if(object exp);

bool is_begin(const object exp);
object begin_actions(object exp);
bool is_last_exp(object seq);
object first_exp(object seq);
object rest_exps(object seq);

bool is_application(const object exp);
object operator(const object exp);
object operands(const object exp);
bool is_no_operands(const object exp);
object first_operand(const object ops);
object rest_operands(const object ops);

// cond

//
// §4.1.3 Evaluator Data Structures
//

bool is_compound_procedure(const object exp);

object make_procedure(object parameters, object body, object env);
object procedure_parameters(object p);
object procedure_body(object p);
object procedure_environment(object p);

object extend_environment(object vars, object vals, object base_env);
object lookup_variable_value(const object var, object env);

void define_variable(object var, object val, object env);

//
// §4.1.4 Running the Evaluator as a Program
//

object setup_environment(bool init);
object the_global_environment(bool init);
object apply_primitive_procedure(object proc, object args);

#endif
