#include "explicit-control-evaluator.h"

#include "hardware.h"
#include "list.h"
#include "metacircular-procedures.h"
#include "object.h"
#include "primitive-procedures.h"
#include "sicp-error.h"

#include <stdbool.h>

static void display_time(object start)
{
	object end = Runtime(EMPTY_LIST);
	object elapsed = Sub(list2(end, start));
	object secs = Seconds(list1(elapsed));
	Display(list1(of_string("[time: ", NO_META_DATA)));
	Display(list1(secs));
	Display(list1(of_string("s] ", NO_META_DATA)));
}

//
// let
//
// Todo: arg checking
// Todo: move to separate file?
#include "args.h"

static object make_call(object proc, object args)
{
	return cons(proc, args, NO_META_DATA);
}

static object let_pairs(object exp)
{
	return cadr(exp);
}

static object let_params(object exp)
{
	return map(car, let_pairs(exp));
}

static object let_body(object exp)
{
	return cddr(exp);
}

static object let_values(object exp)
{
	return map(cadr, let_pairs(exp));
}

static object let_to_combination(object exp)
{
	return make_call(make_lambda(let_params(exp), let_body(exp)),
			 let_values(exp));
}

// Integers we can switch on to select goto destination
enum label {
	LABEL_EV_APPL_ACCUM_LAST_ARG,
	LABEL_EV_APPL_ACCUMULATE_ARG,
	LABEL_EV_APPL_DID_OPERATOR,
	LABEL_EV_DEFINITION_1,
	LABEL_EV_IF_DECIDE,
	LABEL_EV_SEQUENCE_CONTINUE,
	LABEL_EV_TIME_DONE,
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

static struct object _EV_DEFINITION_1 = GOTO_LABEL(LABEL_EV_DEFINITION_1);
const object EV_DEFINITION_1 = &_EV_DEFINITION_1;

static struct object _EV_IF_DECIDE = GOTO_LABEL(LABEL_EV_IF_DECIDE);
const object EV_IF_DECIDE = &_EV_IF_DECIDE;

static struct object _EV_SEQUENCE_CONTINUE =
	GOTO_LABEL(LABEL_EV_SEQUENCE_CONTINUE);
const object EV_SEQUENCE_CONTINUE = &_EV_SEQUENCE_CONTINUE;

static struct object _EV_TIME_DONE = GOTO_LABEL(LABEL_EV_TIME_DONE);
const object EV_TIME_DONE = &_EV_TIME_DONE;

static struct object _EVAL_DISPATCH = GOTO_LABEL(LABEL_EVAL_DISPATCH);
const object EVAL_DISPATCH = &_EVAL_DISPATCH;

static struct object _RETURN_CALLER = GOTO_LABEL(LABEL_RETURN_CALLER);
const object RETURN_CALLER = &_RETURN_CALLER;

//
// §5.1.4  Using a Stack to Implement Recursion
// 	https://www.sicp-book.com/book-Z-H-31.html#%_sec_5.1.4
//

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

static object eval(const core core)
{
	object disp_expr; // Emscripten won't allow declaration to follow label
eval_dispatch:
	disp_expr = core->exp;
	RETURN_IF_ERROR(disp_expr);

	if (is_self_evaluating(disp_expr)) {
		goto ev_self_eval;
	}
	if (is_variable(disp_expr)) {
		goto ev_variable;
	}
	if (is_pair(disp_expr)) {
		object head = car(disp_expr);
		if (is_symbol(head)) {
			if (head == QUOTE) {
				goto ev_quoted;
			}
			if (head == DEFINE) {
				goto ev_definition;
			}
			if (head == IF) {
				goto ev_if;
			}
			if (head == COND) {
				goto ev_cond;
			}
			if (head == LAMBDA) {
				goto ev_lambda;
			}
			if (head == LET) {
				goto ev_let;
			}
			if (head == BEGIN) {
				goto ev_begin;
			}
			if (head == TIME) {
				goto ev_time;
			}
		}
		goto ev_application;
	}
	eprintf("Unknown expression type: '%s'.", to_text(disp_expr));
	return of_error_kind(ERROR_UNKNOWN_EXPRESSION_TYPE, NO_META_DATA);

	//
	// Evaluating simple expressions
	//

ev_self_eval:
	core->val = core->exp;
	goto goto_cont;

ev_variable:
	core->val = lookup_variable_value(core->exp, core->env);
	RETURN_IF_ERROR(core->val);
	goto goto_cont;

ev_quoted:
	core->val = text_of_quotation(core->exp);
	RETURN_IF_ERROR(core->val);
	goto goto_cont;

ev_lambda:
	core->unev = lambda_parameters(core->exp);
	RETURN_IF_ERROR(core->unev);
	core->exp = lambda_body(core->exp);
	RETURN_IF_ERROR(core->exp);
	core->val = make_procedure(core->unev, core->exp, core->env);
	goto goto_cont;

	//
	// Evaluating procedure applications
	//

ev_application:
	save(core, core->cont);
	save(core, core->env);
	core->unev = operands(core->exp);
	save(core, core->unev);
	core->exp = operator(core->exp);
	core->cont = EV_APPL_DID_OPERATOR;
	goto eval_dispatch;

ev_appl_did_operator:
	RETURN_IF_ERROR(core->val);
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
	core->exp = first_operand(core->unev);
	if (is_last_operand(core->unev)) {
		goto ev_appl_last_arg;
	}
	save(core, core->env);
	save(core, core->unev);
	core->cont = EV_APPL_ACCUMULATE_ARG;
	goto eval_dispatch;

ev_appl_accumulate_arg:
	RETURN_IF_ERROR(core->val);
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
	RETURN_IF_ERROR(core->val);
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
	if (is_compound_procedure(core->proc)) {
		goto compound_apply;
	}

	eprintf("Unkown procedure type: '%s'", to_text(core->proc));
	return of_error_kind(ERROR_UNKNOWN_PROCEDURE_TYPE, NO_META_DATA);

primitive_apply:
	core->val = apply_primitive_procedure(core->proc, core->argl);
	core->cont = restore(core);
	goto goto_cont;

compound_apply:
	core->unev = procedure_parameters(core->proc);
	core->env = procedure_environment(core->proc);
	core->env = extend_environment(core->unev, core->argl, core->env);
	core->unev = procedure_body(core->proc);
	goto ev_sequence;

	//
	// §5.4.2 Sequence Evaluation and Tail Recursion
	// 	https://www.sicp-book.com/book-Z-H-34.html#%_sec_5.4.2
	//

ev_begin:
	core->unev = begin_actions(core->exp);
	RETURN_IF_ERROR(core->unev);
	save(core, core->cont);
	goto ev_sequence;

ev_sequence:
	core->exp = first_exp(core->unev);
	RETURN_IF_ERROR(core->exp);
	if (is_last_exp(core->unev)) {
		goto ev_sequence_last_exp;
	}
	save(core, core->unev);
	save(core, core->env);
	core->cont = EV_SEQUENCE_CONTINUE;
	goto eval_dispatch;

ev_sequence_continue:
	RETURN_IF_ERROR(core->val);
	core->env = restore(core);
	core->unev = restore(core);
	core->unev = rest_exps(core->unev);
	goto ev_sequence;

ev_sequence_last_exp:
	core->cont = restore(core);
	goto eval_dispatch;

	//
	// §5.4.3 Conditionals, Assignments, and Definitions
	// 	https://www.sicp-book.com/book-Z-H-34.html#%_sec_5.4.3
	//

ev_if:
	save(core, core->exp);
	save(core, core->env);
	save(core, core->cont);
	core->cont = EV_IF_DECIDE;
	core->exp = if_predicate(core->exp);
	RETURN_IF_ERROR(core->exp);
	goto eval_dispatch;

ev_if_decide:
	RETURN_IF_ERROR(core->val);
	core->cont = restore(core);
	core->env = restore(core);
	core->exp = restore(core);
	if (!is_false(core->val)) {
		goto ev_if_consequent;
	}
	goto ev_if_alternative;

ev_if_alternative:
	core->exp = if_alternative(core->exp);
	goto eval_dispatch;

ev_if_consequent:
	core->exp = if_consequent(core->exp);
	goto eval_dispatch;

	//

ev_cond:
	core->exp = cond_to_if(core->exp);
	RETURN_IF_ERROR(core->exp);
	goto eval_dispatch;

	//

ev_definition:
	core->unev = definition_variable(core->exp);
	save(core, core->unev);
	core->exp = definition_value(core->exp);
	save(core, core->env);
	save(core, core->cont);
	core->cont = EV_DEFINITION_1;
	goto eval_dispatch;

ev_definition_1:
	RETURN_IF_ERROR(core->val);
	core->cont = restore(core);
	core->env = restore(core);
	core->unev = restore(core);
	define_variable(core->unev, core->val, core->env);
	core->val = core->unev;
	goto goto_cont;

	//
	// Extras
	//

ev_let:
	core->exp = let_to_combination(core->exp);
	goto ev_application;

ev_time:
	save(core, core->unev);
	core->unev = Runtime(EMPTY_LIST);
	save(core, core->unev);
	save(core, core->cont);
	core->cont = EV_TIME_DONE;
	core->exp = cons(BEGIN, cdr(core->exp), NO_META_DATA);
	goto eval_dispatch;

ev_time_done:
	core->cont = restore(core);
	core->unev = restore(core);
	display_time(core->unev);
	core->unev = restore(core);
	goto goto_cont;

return_caller:
	return core->val;

goto_cont:
	switch (to_label(core->cont)) {
	case LABEL_EV_APPL_ACCUMULATE_ARG:
		goto ev_appl_accumulate_arg;
	case LABEL_EV_APPL_ACCUM_LAST_ARG:
		goto ev_appl_accum_last_arg;
	case LABEL_EV_APPL_DID_OPERATOR:
		goto ev_appl_did_operator;
	case LABEL_EV_DEFINITION_1:
		goto ev_definition_1;
	case LABEL_EV_IF_DECIDE:
		goto ev_if_decide;
	case LABEL_EV_SEQUENCE_CONTINUE:
		goto ev_sequence_continue;
	case LABEL_EV_TIME_DONE:
		goto ev_time_done;
	case LABEL_EVAL_DISPATCH:
		goto eval_dispatch;
	case LABEL_RETURN_CALLER:
		goto return_caller;
	default:
		inyim("Unexpected label in ec eval: '%d'",
		      to_label(core->cont));
		exit(1); // keep compiler happy
	}
}

object ec_eval(object expr, object env)
{
	struct core _core;
	core core = &_core;
	core_init(core);

	core->exp = expr;
	core->env = env;
	core->cont = RETURN_CALLER;
	return eval(core);
}

object EC_Eval(object expr)
{
	return ec_eval(expr, the_global_environment(true));
}
