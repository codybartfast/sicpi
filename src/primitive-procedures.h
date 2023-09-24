#ifndef H_PRIMITIVE_PROCEDURES
#define H_PRIMITIVE_PROCEDURES

#include "list.h"
#include "object.h"

object Display(object args);
object Newline(object args);
object Print(object args);
object Print_Lines(object args);
object Void(object args);

//
// ARITHMETIC
//

object Add(object args);
object Sub(object args);
object Mul(object args);
object Div(object args);

//
// Comparison
//

object Greater_Than(object args);
object Less_Than(object args);
object Equal(object args);

//
// Logic
//

object And(object args);
object Or(object args);
object Not(object args);

#endif
