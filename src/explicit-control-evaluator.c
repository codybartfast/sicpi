#include "explicit-control-evaluator.h"

#include "hardware.h"
#include "metacircular-procedures.h"
#include "object.h"
#include "sicp-error.h"

#include <stdbool.h>

// Integers to represent goto destinations
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

// save and restore
// 	https://www.sicp-book.com/book-Z-H-31.html#%_idx_5574

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

// explicit-control-evaluator
// 	https://www.sicp-book.com/book-Z-H-4.html#%_toc_%_sec_5.4

static object eval(core core)
{
	enum label label;

	// 5.4.1  The Core of the Explicit-Control Evaluator
	//	https://www.sicp-book.com/book-Z-H-34.html#%_sec_5.4.1

eval_dispatch:
	object disp_expr = core->expr;
	if (is_self_evaluating(disp_expr))
		goto ev_self_eval;
	else {
		inyim("Unexpected expression for ec eval_dispatch, obj: '%s'",
		      to_text(disp_expr));
	}

	// Evaluating simple expressions
	//	https://www.sicp-book.com/book-Z-H-34.html#%_sec_Temp_768

ev_self_eval:
	core->val = core->expr;
	goto goto_continue;

	// As we're not running a repl we can't goto 'read-eval-print-loop' so
	// we instead just return to the caller (i.e. stop the machine)

return_caller:
	return core->val;

	// Goto
	//	https://www.sicp-book.com/book-Z-H-31.html#%_idx_5514

goto_continue:
	label = to_goto_label(core->cont);
	goto goto_label;

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
	core->env = EMPTY_LIST;
	core->cont = RETURN_CALLER;
	return eval(core);
}
