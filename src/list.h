#ifndef H_LIST
#define H_LIST

#include "object.h"

object listv(const object item1, ...);
object list1(const object item);
object list2(const object item1, const object item2);
object list3(const object item1, const object item2, const object item3);
object list4(const object item1, const object item2, const object item3,
	     const object item4);

#endif
