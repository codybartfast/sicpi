#include "metacircular-procedures.h"

// self-evaluating?
// 	https://www.sicp-book.com/book-Z-H-26.html#%_idx_4282

inline bool is_self_evaluating(object obj)
{
	return is_number(obj) || is_string(obj);
}
