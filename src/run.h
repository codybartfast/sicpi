#ifndef RUN_H
#define RUN_H

#include "object.h"
#include "source.h"

object load(source src);

object run(object program);

#endif
