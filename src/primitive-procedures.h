#ifndef H_PRIMITIVE_PROCEDURES
#define H_PRIMITIVE_PROCEDURES

#include "object.h"

object Display(object args);
object Newline(object args);
object DisplayN(object args);

//
// ARITHMETIC
//

object Add(object args);
object Sub(object args);
object Mul(object args);
object Div(object args);

#endif
