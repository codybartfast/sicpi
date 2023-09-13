#include "explicit-control-evaluator.h"

#include "hardware.h"
#include "metacircular-procedures.h"
#include "object.h"
#include "sicp-error.h"

#include <stdbool.h>

#define RETURN_IF_ERROR(OBJ)                                                   \
	if (is_error(OBJ))                                                     \
	return OBJ

// Integers we can switch on to select goto destination
enum label { LABEL_EVAL_DISPATCH, LABEL_RETURN_CALLER };

#define GOTO_LABEL(number)                                                     \
	{                                                                      \
		GOTO_LABEL_VALUE_KIND, NO_META_DATA,                           \
		{                                                              \
			.integer = number                                      \
		}                                                              \
	}

// Goto objects we can put on the stack

static struct object _RETURN_CALLER = GOTO_LABEL(LABEL_RETURN_CALLER);
const object RETURN_CALLER = &_RETURN_CALLER;

static struct object _EVAL_DISPATCH = GOTO_LABEL(LABEL_EVAL_DISPATCH);
const object EVAL_DISPATCH = &_EVAL_DISPATCH;

//
// §5.1.4  Using a Stack to Implement Recursion
// 	https://www.sicp-book.com/book-Z-H-31.html#%_sec_5.1.4

inline void save(core core, object obj)
{
	core->stack = cons(obj, core->stack, NO_META_DATA);
}

inline object restore(core core)
{
	object rtn = car(core->stack);
	core->stack = cdr(core->stack);
	return rtn;
}

// §5.4.1  The Core of the Explicit-Control Evaluator
// 	https://www.sicp-book.com/book-Z-H-34.html#%_sec_5.4

static object eval(core core)
{
	enum label label;

eval_dispatch:
	object disp_expr = core->expr;
	if (is_self_evaluating(disp_expr))
		goto ev_self_eval;
	if (is_variable(disp_expr))
		goto ev_variable;
	if (is_pair(disp_expr)) {
		object head = car(disp_expr);
		if (is_symbol(head)) {
			if (head == QUOTE) {
				goto ev_quoted;
			}
		}
	}
	return of_error_kind(ERROR_UNKNOWN_EXPRESSION_TYPE, NO_META_DATA);

	//
	// Evaluating simple expressions

ev_self_eval:
	core->val = core->expr; // Todo: disp_expr?
	label = to_label(core->cont);
	goto goto_label;

ev_variable:
	core->val = lookup_variable_value(core->expr, core->env);
	RETURN_IF_ERROR(core->val);
	label = to_label(core->cont);
	goto goto_label;

ev_quoted:
	core->val = car(cdr(core->expr));
	label = to_label(core->cont);
	goto goto_label;

	// Not from the book:
	//
	// As we're not running a REPL we can't goto 'read-eval-print-loop' when
	// finished so we instead return the current value to the calling C
	// function

return_caller:
	return core->val;

goto_label:
	switch (label) {
	case LABEL_RETURN_CALLER:
		goto return_caller;
	case LABEL_EVAL_DISPATCH:
		goto eval_dispatch;
	default:
		inyim("Unexpected label in ec eval: '%d'", label);
		exit(1); // keep compiler happy
	}
}

object EC_Eval(object expr)
{
	struct core _core;
	core core = &_core;
	core_init(core);

	core->expr = expr;
	core->env = the_global_environment();
	core->cont = RETURN_CALLER;
	return eval(core);
}
