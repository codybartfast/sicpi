#include "dialect.h"

#include "explicit-control-evaluator.h"
#include "object.h"
#include "parser.h"
#include "primitive-procedures.h"
#include "source.h"
#include "sicp-error.h"

object eval_string(char *name, char *code, object env)
{
	source src = source_string(code, name);
	object program = parse_source(src);
	object rslt = ec_eval(program, env);
	source_close(src);
	return rslt;
}

object make_primitive_procedures_list(void)
{
	meta_data NMD = NO_META_DATA;
	return listv( // this comment just keep separate line
		list2(of_name("+", NMD), of_func(Add, NMD)),
		list2(of_name("-", NMD), of_func(Sub, NMD)),
		list2(of_name("*", NMD), of_func(Mul, NMD)),
		list2(of_name("/", NMD), of_func(Div, NMD)),
		list2(of_name("abs", NMD), of_func(Abs, NMD)),

		list2(of_name(">", NMD), of_func(Greater_Than, NMD)),
		list2(of_name("<", NMD), of_func(Less_Than, NMD)),
		list2(of_name("=", NMD), of_func(Equal, NMD)),

		list2(of_name("and", NMD), of_func(And, NMD)),
		list2(of_name("or", NMD), of_func(Or, NMD)),
		list2(of_name("not", NMD), of_func(Not, NMD)),

		list2(of_name("display", NMD), of_func(Display, NMD)),
		list2(of_name("newline", NMD), of_func(Newline, NMD)),
		list2(of_name("print", NMD), of_func(Print, NMD)),
		list2(of_name("print-lines", NMD), of_func(Print_Lines, NMD)),
		list2(of_name("void", NMD), of_func(Void, NMD)),

		VA_TERM); // this comment just to keep separate line
}

static void define_new_variable(object var, object val, object env)
{
	object frame = car(env);

	set_car(frame, cons(var, car(frame), NO_META_DATA));
	set_cdr(frame, cons(val, cdr(frame), NO_META_DATA));
}

static bool is_dialect_set(object env)
{
	return car(car(env)) != EMPTY_LIST;
}

void _set_dialect(object env)
{
	object pp_list = make_primitive_procedures_list();
	object pp;
	for (; pp_list != EMPTY_LIST; pp_list = cdr(pp_list)) {
		pp = car(pp_list);
		define_new_variable(car(pp), cadr(pp), env);
	}
	define_new_variable(TRUE, TRUE_VALUE, env);
	define_new_variable(FALSE, FALSE_VALUE, env);

	eval_string("dialect:square", "(define (square x) (* x x))", env);
}

void set_dialect(object env)
{
	if (is_dialect_set(env)) {
		inyim("Attempted to set dialect twice!");
	}
	_set_dialect(env);
}

void set_dialect_if_needed(object env)
{
	if (is_dialect_set(env)) {
		return;
	}
	_set_dialect(env);
}