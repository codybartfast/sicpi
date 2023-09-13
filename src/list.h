#ifndef H_LIST
#define H_LIST

#include "object.h"

//
// Constructors
// =============================================================================
//

object listv(const object item1, ...);
object list1(const object item);
object list2(const object item1, const object item2);
object list3(const object item1, const object item2, const object item3);
object list4(const object item1, const object item2, const object item3,
	     const object item4);

//
// Utils
// =============================================================================
//

int length(object lst);

object map(object (*mapper)(object item), object lst);

//
// Accessors
// =============================================================================
//

object caar(object lst);
object cadr(object lst);
object cdar(object lst);
object cddr(object lst);

object caaar(object lst);
object caadr(object lst);
object cadar(object lst);
object caddr(object lst);
object cdaar(object lst);
object cdadr(object lst);
object cddar(object lst);
object cdddr(object lst);

object caaaar(object lst);
object caaadr(object lst);
object caadar(object lst);
object caaddr(object lst);
object cadaar(object lst);
object cadadr(object lst);
object caddar(object lst);
object cadddr(object lst);
object cdaaar(object lst);
object cdaadr(object lst);
object cdadar(object lst);
object cdaddr(object lst);
object cddaar(object lst);
object cddadr(object lst);
object cdddar(object lst);
object cddddr(object lst);

#endif
