#ifndef H_ARGS
#define H_ARGS

#include "object.h"

#define SET_ARGS_0(CALLER, ARGS, AT_LEAST)                                     \
	object SET_ARGS_RESULT = set_args(CALLER, &ARGS, AT_LEAST, 0);         \
	RETURN_IF_ERROR(SET_ARGS_RESULT);

#define SET_ARGS_1(CALLER, ARGS, AT_LEAST)                                     \
	object arg0;                                                           \
	object SET_ARGS_RESULT = set_args(CALLER, &ARGS, AT_LEAST, 1, &arg0);  \
	RETURN_IF_ERROR(SET_ARGS_RESULT);

#define SET_ARGS_2(CALLER, ARGS, AT_LEAST)                                     \
	object arg0;                                                           \
	object arg1;                                                           \
	object SET_ARGS_RESULT =                                               \
		set_args(CALLER, &ARGS, AT_LEAST, 2, &arg0, &arg1);            \
	RETURN_IF_ERROR(SET_ARGS_RESULT);

#define SET_ARGS_3(CALLER, ARGS, AT_LEAST)                                     \
	object arg0;                                                           \
	object arg1;                                                           \
	object arg2;                                                           \
	object SET_ARGS_RESULT =                                               \
		set_args(CALLER, &ARGS, AT_LEAST, 3, &arg0, &arg1, &arg2);     \
	RETURN_IF_ERROR(SET_ARGS_RESULT);

#define SET_ARGS_4(CALLER, ARGS, AT_LEAST)                                     \
	object arg0;                                                           \
	object arg1;                                                           \
	object arg2;                                                           \
	object arg3;                                                           \
	object SET_ARGS_RESULT = set_args(CALLER, &ARGS, AT_LEAST, 4, &arg0,   \
					  &arg1, &arg2, &arg3);                \
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
#define ARGS_AT_LEAST_4(CALLER, ARGS) SET_ARGS_4(CALLER, ARGS, true)

object set_args(char *caller, struct object **args_p, bool at_least, int count,
		...);

#endif
