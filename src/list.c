#include "list.h"

#include "sicp-error.h"

#include <stdarg.h>

//
// Constructors
// =============================================================================
//

// variadic list constructor.  Last argument must be EMPTY_LIST
object listv(const object head, ...)
{
	object lst;
	object end;
	object item;

	if (head == VA_TERM) {
		return EMPTY_LIST;
	}
	lst = end = cons(head, EMPTY_LIST, NO_META_DATA);

	va_list tail;
	va_start(tail, head);
	while ((item = va_arg(tail, struct object *)) != VA_TERM) {
		object pair = cons(item, EMPTY_LIST, NO_META_DATA);
		set_cdr(end, pair);
		end = pair;
	}
	va_end(tail);
	return lst;
}

object list1(const object item)
{
	return cons(item, EMPTY_LIST, NO_META_DATA);
}

object list2(const object item1, const object item2)
{
	return cons(item1, cons(item2, EMPTY_LIST, NO_META_DATA), NO_META_DATA);
}

object list3(const object item1, const object item2, const object item3)
{
	return listv(item1, item2, item3, VA_TERM);
}

object list4(const object item1, const object item2, const object item3,
	     const object item4)
{
	return listv(item1, item2, item3, item4, VA_TERM);
}

//
// Utils
// =============================================================================
//

int length(object lst)
{
	int len = 0;
	for (; lst != EMPTY_LIST; lst = cdr(lst), len++)
		;
	return len;
}

object map(object (*mapper)(object item), object lst)
{
	if (lst == EMPTY_LIST) {
		return lst;
	}

	object end;
	object mapped = end = cons(mapper(car(lst)), EMPTY_LIST, NO_META_DATA);

	for (lst = cdr(lst); lst != EMPTY_LIST; lst = cdr(lst)) {
		object pair = cons(mapper(car(lst)), EMPTY_LIST, NO_META_DATA);
		set_cdr(end, pair);
		end = pair;
	}
	return mapped;
}

//
// Accessors
// =============================================================================
//

// TWO

object caar(object lst)
{
	return car(car(lst));
}

object cadr(object lst)
{
	return car(cdr(lst));
}
object cdar(object lst)
{
	return cdr(car(lst));
}

object cddr(object lst)
{
	return cdr(cdr(lst));
}

// THREE

object caaar(object lst)
{
	return car(car(car(lst)));
}

object caadr(object lst)
{
	return car(car(cdr(lst)));
}
object cadar(object lst)
{
	return car(cdr(car(lst)));
}

object caddr(object lst)
{
	return car(cdr(cdr(lst)));
}

object cdaar(object lst)
{
	return cdr(car(car(lst)));
}

object cdadr(object lst)
{
	return cdr(car(cdr(lst)));
}
object cddar(object lst)
{
	return cdr(cdr(car(lst)));
}

object cdddr(object lst)
{
	return cdr(cdr(cdr(lst)));
}

// FOUR

object caaaar(object lst)
{
	return car(car(car(car(lst))));
}

object caaadr(object lst)
{
	return car(car(car(cdr(lst))));
}
object caadar(object lst)
{
	return car(car(cdr(car(lst))));
}

object caaddr(object lst)
{
	return car(car(cdr(cdr(lst))));
}

object cadaar(object lst)
{
	return car(cdr(car(car(lst))));
}

object cadadr(object lst)
{
	return car(cdr(car(cdr(lst))));
}
object caddar(object lst)
{
	return car(cdr(cdr(car(lst))));
}

object cadddr(object lst)
{
	return car(cdr(cdr(cdr(lst))));
}

object cdaaar(object lst)
{
	return cdr(car(car(car(lst))));
}

object cdaadr(object lst)
{
	return cdr(car(car(cdr(lst))));
}
object cdadar(object lst)
{
	return cdr(car(cdr(car(lst))));
}

object cdaddr(object lst)
{
	return cdr(car(cdr(cdr(lst))));
}

object cddaar(object lst)
{
	return cdr(cdr(car(car(lst))));
}

object cddadr(object lst)
{
	return cdr(cdr(car(cdr(lst))));
}
object cdddar(object lst)
{
	return cdr(cdr(cdr(car(lst))));
}

object cddddr(object lst)
{
	return cdr(cdr(cdr(cdr(lst))));
}
