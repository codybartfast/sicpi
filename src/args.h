#ifndef H_ARGS
#define H_ARGS

#define ARGS_AT_LEAST_1(NAME)                                                  \
	object ARGS = args;                                                    \
	if (!is_pair(ARGS)) {                                                  \
		eprintf("'%s' expected 1 argument but got 0 arguments.",       \
			NAME);                                                 \
		return (of_error_kind(ERROR_INCORRECT_NUMBER_OF_ARGUMENTS,     \
				      NO_META_DATA));                          \
	}                                                                      \
	object arg1 = car(ARGS);                                               \
	ARGS = cdr(ARGS);

#define ARGS_1(NAME)                                                           \
	ARGS_AT_LEAST_1(NAME)                                                  \
                                                                               \
	if (!is_null(ARGS)) {                                                  \
		eprintf("'%s' only expected 1 arguments but got 2 or more.",   \
			NAME);                                                 \
		return (of_error_kind(ERROR_INCORRECT_NUMBER_OF_ARGUMENTS,     \
				      NO_META_DATA));                          \
	}

#endif
