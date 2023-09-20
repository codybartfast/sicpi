#include "metacircular-procedures.h"

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

static inline bool is_tagged_list(object exp, object tag)
{
	return is_pair(exp) && (car(exp) == tag);
}

inline bool is_quoted(const object exp)
{
	return is_tagged_list(exp, QUOTE);
}

inline object text_of_quotation(const object exp)
{
	return cadr(exp);
}

inline bool is_definition(const object exp)
{
	return is_tagged_list(exp, DEFINE);
}

inline object definition_variable(object exp)
{
	return is_symbol(cadr(exp)) ? cadr(exp) : caadr(exp);
}

object definition_value(object exp)
{
	if (is_symbol(cadr(exp))) {
		return caddr(exp);
	} else {
		inyim("make-lambda not implemented");
		exit(1);
	}
}

inline bool is_begin(const object exp)
{
	return is_tagged_list(exp, BEGIN);
}

inline object begin_actions(object exp)
{
	return cdr(exp);
}

inline bool is_last_exp(object seq)
{
	return is_null(cdr(seq));
}

inline object first_exp(object seq)
{
	return car(seq);
}

inline object rest_exps(object seq)
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

object make_primitive_procedures_list(void)
{
	meta_data NMD = NO_META_DATA;
	return listv( // this comment just keep separate line
		list2(of_name("+", NMD), of_func(Add, NMD)),
		list2(of_name("-", NMD), of_func(Sub, NMD)),
		list2(of_name("*", NMD), of_func(Mul, NMD)),
		list2(of_name("/", NMD), of_func(Div, NMD)),
		list2(of_name("display", NMD), of_func(Display, NMD)),
		list2(of_name("newline", NMD), of_func(Newline, NMD)),
		EMPTY_LIST); // this comment just to keep separate line
}

object _primitive_procedures = NULL;
object primitive_procedures(void)
{
	return _primitive_procedures ?
		       _primitive_procedures :
		       (_primitive_procedures =
				make_primitive_procedures_list());
}

object primitive_procedure_names(void)
{
	return map(car, primitive_procedures());
}

object primitive_procedure_objects(void)
{
	return map(cadr, primitive_procedures());
}

object setup_environment(void)
{
	object initial_env = extend_environment(primitive_procedure_names(),
						primitive_procedure_objects(),
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

object apply_primitive_procedure(object proc, object args)
{
	return (*to_func(proc))(args);
}
