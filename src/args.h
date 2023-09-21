#ifndef H_ARGS
#define H_ARGS

#include "object.h"

#define ARGS_0(CALLER)                                                         \
	object SET_ARGS_RESULT = set_args(CALLER, args, 0, false);             \
	RETURN_IF_ERROR(SET_ARGS_RESULT);

#define ARGS_1(CALLER)                                                         \
	object arg1 = NULL;                                                    \
	object SET_ARGS_RESULT = set_args(CALLER, args, 1, false, &arg1);      \
	RETURN_IF_ERROR(SET_ARGS_RESULT);

object set_args(char *caller, object args, int count, bool at_least, ...);

#endif
