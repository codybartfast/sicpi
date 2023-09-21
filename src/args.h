#ifndef H_ARGS
#define H_ARGS

#include "object.h"

#define SET_ARGS_0(CALLER, ARGS, AT_LEAST)                                     \
	object SET_ARGS_RESULT = set_args(CALLER, &ARGS, 0, AT_LEAST);         \
	RETURN_IF_ERROR(SET_ARGS_RESULT);

#define SET_ARGS_1(CALLER, ARGS, AT_LEAST)                                     \
	object arg1;                                                           \
	object SET_ARGS_RESULT = set_args(CALLER, &ARGS, 1, AT_LEAST, &arg1);  \
	RETURN_IF_ERROR(SET_ARGS_RESULT);

#define SET_ARGS_2(CALLER, ARGS, AT_LEAST)                                     \
	object arg1;                                                           \
	object arg2;                                                           \
	object SET_ARGS_RESULT =                                               \
		set_args(CALLER, &ARGS, 2, AT_LEAST, &arg1, &arg2);            \
	RETURN_IF_ERROR(SET_ARGS_RESULT);

#define SET_ARGS_3(CALLER, ARGS, AT_LEAST)                                     \
	object arg1;                                                           \
	object arg2;                                                           \
	object arg3;                                                           \
	object SET_ARGS_RESULT =                                               \
		set_args(CALLER, &ARGS, 3, AT_LEAST, &arg1, &arg2, &arg3);     \
	RETURN_IF_ERROR(SET_ARGS_RESULT);

#define SET_ARGS_4(CALLER, ARGS, AT_LEAST)                                     \
	object arg1;                                                           \
	object arg2;                                                           \
	object arg3;                                                           \
	object arg4;                                                           \
	object SET_ARGS_RESULT = set_args(CALLER, &ARGS, 4, AT_LEAST, &arg1,   \
					  &arg2, &arg3, &arg4);                \
	RETURN_IF_ERROR(SET_ARGS_RESULT);

#define ARGS_0(CALLER, ARGS) SET_ARGS_0(CALLER, ARGS, false)
#define ARGS_AT_LEAST_0(CALLER, ARGS) SET_ARGS_0(CALLER, ARGS, true)

#define ARGS_1(CALLER, ARGS) SET_ARGS_1(CALLER, ARGS, false)
#define ARGS_AT_LEAST_1(CALLER, ARGS) SET_ARGS_1(CALLER, ARGS, true)

#define ARGS_2(CALLER, ARGS) SET_ARGS_2(CALLER, ARGS, false)
#define ARGS_AT_LEAST_2(CALLER, ARGS) SET_ARGS_2(CALLER, ARGS, true)

#define ARGS_3(CALLER, ARGS) SET_ARGS_3(CALLER, ARGS, false)
#define ARGS_AT_LEAST_3(CALLER, ARGS) SET_ARGS_3(CALLER, ARGS, true)

#define ARGS_4(CALLER, ARGS) SET_ARGS_4(CALLER, ARGS, false)
#define ARGS_AT_LEAST_4(CALLER, ARGS) SET_ARGS_4 (CALLER, ARGS, true)

object set_args(char *caller, struct object **args_p, int count, bool at_least,
		...);

#endif
