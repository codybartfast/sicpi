#include "dialect.h"

#include "object.h"
#include "primitive-procedures.h"

object make_primitive_procedures_list(void)
{
	meta_data NMD = NO_META_DATA;
	return listv( // this comment just keep separate line
		list2(of_name("+", NMD), of_func(Add, NMD)),
		list2(of_name("-", NMD), of_func(Sub, NMD)),
		list2(of_name("*", NMD), of_func(Mul, NMD)),
		list2(of_name("/", NMD), of_func(Div, NMD)),
		list2(of_name("abs", NMD), of_func(Abs, NMD)),

		list2(of_name(">", NMD), of_func(Greater_Than, NMD)),
		list2(of_name("<", NMD), of_func(Less_Than, NMD)),
		list2(of_name("=", NMD), of_func(Equal, NMD)),

		list2(of_name("and", NMD), of_func(And, NMD)),
		list2(of_name("or", NMD), of_func(Or, NMD)),
		list2(of_name("not", NMD), of_func(Not, NMD)),

		list2(of_name("display", NMD), of_func(Display, NMD)),
		list2(of_name("newline", NMD), of_func(Newline, NMD)),
		list2(of_name("print", NMD), of_func(Print, NMD)),
		list2(of_name("print-lines", NMD), of_func(Print_Lines, NMD)),
		list2(of_name("void", NMD), of_func(Void, NMD)),
		VA_TERM); // this comment just to keep separate line

	/*
	 * Potential convience procedures:
	 * 	square
	 */
}

