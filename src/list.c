#include "list.h"

#include <stdarg.h>

// variadic list constructor.  Last argument must be EMPTY_LIST
object listv(const object head, ...)
{
	object lst;
	object end;
	object item;

	if (head == EMPTY_LIST) {
		return head;
	}
	lst = end = cons(head, EMPTY_LIST, NO_META_DATA);

	va_list tail;
	va_start(tail, head);
	while ((item = va_arg(tail, struct object *)) != EMPTY_LIST) {
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
	return listv(item1, item2, item3, EMPTY_LIST);
}

object list4(const object item1, const object item2, const object item3,
	     const object item4)
{
	return listv(item1, item2, item3, item4, EMPTY_LIST);
}
