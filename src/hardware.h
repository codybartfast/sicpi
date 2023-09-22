#ifndef H_HARDWARE
#define H_HARDWARE

#include "object.h"

typedef struct core {
	// registers
	// 	https://www.sicp-book.com/book-Z-H-34.html#%_sec_Temp_766

	object exp;
	object env;
	object val;
	object cont;
	object proc;
	object argl;
	object unev;

	object stack;
} *core;

void core_init(core core);

#endif
