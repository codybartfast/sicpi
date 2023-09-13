#include "metacircular-procedures.h"

#include "list.h"
#include "sicp-error.h"

//
// §4.1.2 Representing Expressions
// 	https://www.sicp-book.com/book-Z-H-26.html#%_sec_4.1.2

inline bool is_self_evaluating(const object obj)
{
	return is_number(obj) || is_string(obj);
}

inline bool is_variable(const object obj)
{
	return is_symbol(obj);
}

//
// §4.1.3 Evaluator Data Structures
//	https://www.sicp-book.com/book-Z-H-26.html#%_sec_4.1.3

//
// Operations on Environments
//

object enclosing_envronment(object env)
{
	return cdr(env);
}

object first_frame(object env)
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
		inyim("extend_environame (placeholder)");
		exit(1);
		//TODO: scheme error
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
	inyim("lookup_variable_value (placeholder)");
	exit(1);
	// TODO: scheme error
}

void set_variable_value(object var, object val, object env)
{
	while (env != the_empty_environment()) {
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
		env = cdr(env);
	}
	inyim("set_variable_value (placeholder)");
	// TODO: scheme error
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

object setup_environment(void)
{
	// TODO: add primitive procedures
	object initial_env = extend_environment(EMPTY_LIST, EMPTY_LIST,
						the_empty_environment());
	define_variable(of_name("true", NO_META_DATA), TRUE, initial_env);
	define_variable(of_name("false", NO_META_DATA), FALSE, initial_env);
	return initial_env;
}

static object _the_global_environment = NULL;
object the_global_environment(void)
{
	return _the_global_environment ?
		       _the_global_environment :
		       (_the_global_environment = setup_environment());
}
