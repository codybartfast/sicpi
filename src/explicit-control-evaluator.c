#include "explicit-control-evaluator.h"

#include "hardware.h"
#include "list.h"
#include "metacircular-procedures.h"
#include "object.h"
#include "sicp-error.h"

#include <stdbool.h>

// Integers we can switch on to select goto destination
enum label {
	LABEL_EV_APPL_ACCUM_LAST_ARG,
	LABEL_EV_APPL_ACCUMULATE_ARG,
	LABEL_EV_APPL_DID_OPERATOR,
	LABEL_EVAL_DISPATCH,
	LABEL_RETURN_CALLER
};

#define GOTO_LABEL(number)                                                     \
	{                                                                      \
		VK_GOTO_LABEL, NO_META_DATA,                                   \
		{                                                              \
			.integer = number                                      \
		}                                                              \
	}

// Goto objects we can put on the stack

static struct object _EV_APPL_ACCUM_LAST_ARG =
	GOTO_LABEL(LABEL_EV_APPL_ACCUM_LAST_ARG);
const object EV_APPL_ACCUM_LAST_ARG = &_EV_APPL_ACCUM_LAST_ARG;

static struct object _EV_APPL_ACCUMULATE_ARG =
	GOTO_LABEL(LABEL_EV_APPL_ACCUMULATE_ARG);
const object EV_APPL_ACCUMULATE_ARG = &_EV_APPL_ACCUMULATE_ARG;

static struct object _EV_APPL_DID_OPERATOR =
	GOTO_LABEL(LABEL_EV_APPL_DID_OPERATOR);
const object EV_APPL_DID_OPERATOR = &_EV_APPL_DID_OPERATOR;

static struct object _EVAL_DISPATCH = GOTO_LABEL(LABEL_EVAL_DISPATCH);
const object EVAL_DISPATCH = &_EVAL_DISPATCH;

static struct object _RETURN_CALLER = GOTO_LABEL(LABEL_RETURN_CALLER);
const object RETURN_CALLER = &_RETURN_CALLER;

//
// §5.1.4  Using a Stack to Implement Recursion
// 	https://www.sicp-book.com/book-Z-H-31.html#%_sec_5.1.4

static inline void save(core core, object obj)
{
	core->stack = cons(obj, core->stack, NO_META_DATA);
}

static inline object restore(core core)
{
	object rtn = car(core->stack);
	core->stack = cdr(core->stack);
	return rtn;
}

//
// §5.4 footnote 22
//

static object adjoin_arg(object arg, object arg_list)
{
	object new_end = list1(arg);
	if (arg_list == EMPTY_LIST) {
		return new_end;
	}

	object old_end;
	for (old_end = arg_list; cdr(old_end) != EMPTY_LIST;
	     old_end = cdr(old_end))
		;
	set_cdr(old_end, new_end);
	return arg_list;
}

static bool is_last_operand(object ops)
{
	return is_null(cdr(ops));
}

//
// §5.4.1  The Core of the Explicit-Control Evaluator
// 	https://www.sicp-book.com/book-Z-H-34.html#%_sec_5.4
//

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
		goto ev_application;
	}
	return of_error_kind(ERROR_UNKNOWN_EXPRESSION_TYPE, NO_META_DATA);

	//
	// Evaluating simple expressions
	//

ev_self_eval:
	core->val = core->expr;
	label = to_label(core->cont);
	goto goto_label;

ev_variable:
	core->val = lookup_variable_value(core->expr, core->env);
	RETURN_IF_ERROR(core->val);
	label = to_label(core->cont);
	goto goto_label;

ev_quoted:
	core->val = text_of_quotation(core->expr);
	label = to_label(core->cont);
	goto goto_label;

	//
	// Evaluating procedure applications
	//

ev_application:
	save(core, core->cont);
	save(core, core->env);
	core->unev = operands(core->expr);
	save(core, core->unev);
	core->expr = operator(core->expr);
	core->cont = EV_APPL_DID_OPERATOR;
	goto eval_dispatch;

ev_appl_did_operator:
	core->unev = restore(core);
	core->env = restore(core);
	core->argl = EMPTY_LIST;
	core->proc = core->val;
	if (is_no_operands(core->unev)) {
		goto apply_dispatch;
	}
	save(core, core->proc);

ev_appl_operand_loop:
	save(core, core->argl);
	core->expr = first_operand(core->unev);
	if (is_last_operand(core->unev)) {
		goto ev_appl_last_arg;
	}
	save(core, core->env);
	save(core, core->unev);
	core->cont = EV_APPL_ACCUMULATE_ARG;
	goto eval_dispatch;

ev_appl_accumulate_arg:
	core->unev = restore(core);
	core->env = restore(core);
	core->argl = restore(core);
	core->argl = adjoin_arg(core->val, core->argl);
	core->unev = rest_operands(core->unev);
	goto ev_appl_operand_loop;

ev_appl_last_arg:
	core->cont = EV_APPL_ACCUM_LAST_ARG;
	goto eval_dispatch;

ev_appl_accum_last_arg:
	core->argl = restore(core);
	core->argl = adjoin_arg(core->val, core->argl);
	core->proc = restore(core);
	goto apply_dispatch;

	//
	// Procedure Application
	//

apply_dispatch:
	if (is_primitive_procedure(core->proc)) {
		goto primitive_apply;
	}
	return of_error_kind(ERROR_UNKNOWN_PROCEDURE_TYPE, NO_META_DATA);

primitive_apply:
	core->val = apply_primitive_procedure(core->proc, core->argl);
	core->cont = restore(core);
	label = to_label(core->cont);
	goto goto_label;

	//
	// Extras
	//
	// As we're not running a REPL we can't goto 'read-eval-print-loop' when
	// finished so we instead return the current value to the calling C
	// function

return_caller:
	return core->val;

goto_label:
	switch (label) {
	case LABEL_EV_APPL_ACCUMULATE_ARG:
		goto ev_appl_accumulate_arg;
	case LABEL_EV_APPL_ACCUM_LAST_ARG:
		goto ev_appl_accum_last_arg;
	case LABEL_EVAL_DISPATCH:
		goto eval_dispatch;
	case LABEL_EV_APPL_DID_OPERATOR:
		goto ev_appl_did_operator;
	case LABEL_RETURN_CALLER:
		goto return_caller;
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
