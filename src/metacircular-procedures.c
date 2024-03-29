#include "metacircular-procedures.h"

#include "args.h"
#include "dialect.h"
#include "list.h"
#include "primitive-procedures.h"
#include "sicp-error.h"

//
// §4.1.2 Representing Expressions
// 	https://www.sicp-book.com/book-Z-H-26.html#%_sec_4.1.2

inline bool is_self_evaluating(const object exp)
{
	return is_number(exp) || is_string(exp);
}

inline bool is_variable(const object exp)
{
	return is_symbol(exp);
}

static inline bool is_tagged_list(const object exp, const object tag)
{
	return is_pair(exp) && (car(exp) == tag);
}

inline bool is_quoted(const object exp)
{
	return is_tagged_list(exp, QUOTE);
}

object text_of_quotation(object exp)
{
	ARGS_2("text_of_quotation", exp);

	return arg1;
}

// assign

bool is_lambda(const object exp)
{
	return is_tagged_list(exp, LAMBDA);
}

object lambda_parameters(object exp)
{
	ARGS_AT_LEAST_2("lambda_parameters", exp);

	return arg1;
}

object lambda_body(object exp)
{
	ARGS_AT_LEAST_2("lambda_body", exp);

	return exp;
}

object make_lambda(const object parameters, const object body)
{
	return cons(LAMBDA, cons(parameters, body, NO_META_DATA), NO_META_DATA);
}

//

inline bool is_definition(const object exp)
{
	return is_tagged_list(exp, DEFINE);
}

object definition_variable(object exp)
{
	ARGS_AT_LEAST_2("definition_variable", exp);

	return is_symbol(arg1) ? arg1 : car(arg1);
}

object definition_value(object exp)
{
	ARGS_AT_LEAST_3("definition_value", exp);

	if (is_symbol(arg1)) {
		return arg2;
	} else {
		return make_lambda(cdr(arg1), cons(arg2, exp, NO_META_DATA));
	}
}

//

inline bool is_if(const object exp)
{
	return is_tagged_list(exp, IF);
}

object if_predicate(object exp)
{
	ARGS_AT_LEAST_2("if_predicate", exp);

	return arg1;
}

object if_consequent(object exp)
{
	ARGS_AT_LEAST_3("if_consequent", exp);

	return arg2;
}

object if_alternative(object exp)
{
	ARGS_AT_LEAST_3("if_alternative", exp);

	return is_pair(exp) ? car(exp) : FALSE;
}

// Cond

object make_if(object predicate, object consequent, object alternative)
{
	return list4(IF, predicate, consequent, alternative);
}

object make_begin(const object exp)
{
	return cons(BEGIN, exp, NO_META_DATA);
}

object sequence_to_exp(const object seq)
{
	if (is_null(seq)) {
		// Book says return seq which is the empty list ()
		// but this gets creates an error when evaluated. We can't
		// return OK either, but could return a symbol 'ok if we
		// define it as a variable with value OK.
		// Other alternatives could be:
		// 	false
		//      (quote empty_list)
		return list1(of_name("void", NO_META_DATA));
	}
	if (is_last_exp(seq)) {
		return first_exp(seq);
	}
	return make_begin(seq);
}

object cond_predicate(object clause)
{
	return car(clause);
}

object cond_actions(object clause)
{
	return cdr(clause);
}

bool is_cond_else_clause(object clause)
{
	return is_eq(cond_predicate(clause), ELSE);
}

object expand_clauses(object clauses)
{
	if (is_null(clauses)) {
		return FALSE;
	}

	object first = car(clauses);
	object rest = cdr(clauses);

	if (!is_pair(first) && !is_null(first)) {
		eprintf("'cond' clause is not valid: %s", to_text(first));
		return of_error_kind(ERROR_COND_CLAUSE_IS_NOT_VALID,
				     NO_META_DATA);
	}

	if (is_cond_else_clause(first)) {
		if (is_null(rest)) {
			return sequence_to_exp(cond_actions(first));
		} else {
			eprintf("'else' is not last cond clause %s.",
				to_text(clauses));
			return of_error_kind(ERROR_ELSE_IS_NOT_LAST_CLAUSE,
					     NO_META_DATA);
		}
	}
	return make_if(cond_predicate(first),
		       sequence_to_exp(cond_actions(first)),
		       expand_clauses(rest));
}

object cond_clauses(object exp)
{
	// Should not need to check is a pair because only able to get here if
	// given a list tagged with 'cond'.
	return cdr(exp);
}

object cond_to_if(object exp)
{
	return expand_clauses(cond_clauses(exp));
}

//

inline bool is_begin(const object exp)
{
	return is_tagged_list(exp, BEGIN);
}

object begin_actions(object exp)
{
	ARGS_AT_LEAST_1("begin_actions", exp);

	return exp;
}

inline bool is_last_exp(const object seq)
{
	return is_null(cdr(seq));
}

object first_exp(const object seq)
{
	return is_pair(seq) ?
		       car(seq) :
		       of_error_kind(ERROR_EMPTY_BEGIN_SEQUENCE, NO_META_DATA);
}

inline object rest_exps(const object seq)
{
	return cdr(seq);
}

//

inline bool is_application(const object exp)
{
	return is_pair(exp);
}

inline object operator(const object exp)
{
	return car(exp);
}

inline object operands(const object exp)
{
	return cdr(exp);
}

inline bool is_no_operands(const object ops)
{
	return is_null(ops);
}

inline object first_operand(const object ops)
{
	return car(ops);
}

inline object rest_operands(const object ops)
{
	return cdr(ops);
}

//
// §4.1.3 Evaluator Data Structures
//	https://www.sicp-book.com/book-Z-H-26.html#%_sec_4.1.3

//
// Operations on Environments
//

bool is_compound_procedure(const object exp)
{
	return is_tagged_list(exp, PROCEDURE);
}

object make_procedure(const object parameters, const object body,
		      const object env)
{
	return list4(PROCEDURE, parameters, body, env);
}

object procedure_parameters(object p)
{
	ARGS_AT_LEAST_2("procedure_parameters", p);

	return arg1;
}

object procedure_body(object p)
{
	ARGS_AT_LEAST_3("procedure_body", p);

	return arg2;
}

object procedure_environment(object p)
{
	ARGS_4("procedure_environment", p);

	return arg3;
}

object enclosing_envronment(const object env)
{
	return cdr(env);
}

object first_frame(const object env)
{
	return car(env);
}

object the_empty_environment(void)
{
	return EMPTY_LIST;
}

object make_frame(object variables, object values)
{
	return cons(variables, values, NO_META_DATA);
}

object frame_variables(object frame)
{
	return car(frame);
}

object frame_values(object frame)
{
	return cdr(frame);
}

void add_binding_to_frame(object var, object val, object frame)
{
	set_car(frame, cons(var, car(frame), NO_META_DATA));
	set_cdr(frame, cons(val, cdr(frame), NO_META_DATA));
}

object extend_environment(object vars, object vals, object base_env)
{
	if (length(vars) == length(vals)) {
		return cons(make_frame(vars, vals), base_env, NO_META_DATA);
	} else {
		inyim("extend_environment was given mismatched lists of vars: %d, and vals: %d.",
		      length(vars), length(vals));
		exit(1);
	}
}

object lookup_variable_value(const object var, object env)
{
	while (env != the_empty_environment()) {
		object frame = first_frame(env);
		object vars = frame_variables(frame);
		object vals = frame_values(frame);
		while (!is_null(vars)) {
			if (is_eq(var, car(vars))) {
				return car(vals);
			}
			vars = cdr(vars);
			vals = cdr(vals);
		}
		env = cdr(env);
	}
	eprintf("Unbound variable '%s'.", to_text(var));
	return of_error_kind(ERROR_UNBOUND_VARIABLE, object_meta_data(var));
}

void define_variable(object var, object val, object env)
{
	object frame = first_frame(env);
	object vars = frame_variables(frame);
	object vals = frame_values(frame);
	while (!is_null(vars)) {
		if (is_eq(var, car(vars))) {
			set_car(vals, val);
			return;
		}
		vars = cdr(vars);
		vals = cdr(vals);
	}
	add_binding_to_frame(var, val, frame);
}

//
// §4.1.4 Running the Evaluator as a Program
//	https://www.sicp-book.com/book-Z-H-26.html#%_sec_4.1.4

object primitive_procedures(void)
{
	return EMPTY_LIST;
}

object primitive_procedure_names(void)
{
	return map(car, primitive_procedures());
}

object primitive_procedure_objects(void)
{
	return map(cadr, primitive_procedures());
}

object setup_environment(bool init)
{
	object initial_env = extend_environment(primitive_procedure_names(),
						primitive_procedure_objects(),
						the_empty_environment());
	if (init) {
		set_dialect_if_needed(initial_env);
	}
	return initial_env;
}

static object _the_global_environment = NULL;
object the_global_environment(bool init)
{
	return _the_global_environment ?
		       _the_global_environment :
		       (_the_global_environment = setup_environment(init));
}

object apply_primitive_procedure(object proc, object args)
{
	return (*to_func(proc))(args);
}
