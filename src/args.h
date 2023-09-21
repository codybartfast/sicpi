#ifndef H_ARGS
#define H_ARGS

#include "object.h"

#define ARGS_0(ARGS, CALLER)                                                 \
	object SET_ARGS_RESULT = set_args(CALLER, &ARGS, 0, false);          \
	RETURN_IF_ERROR(SET_ARGS_RESULT);

#define ARGS_1(ARGS, CALLER)                                                 \
	object arg1 = NULL;                                                    \
	object SET_ARGS_RESULT = set_args(CALLER, &ARGS, 1, false, &arg1);   \
	RETURN_IF_ERROR(SET_ARGS_RESULT);

object set_args(char *caller, struct object **args_p, int count, bool at_least,
		...);

#endif
