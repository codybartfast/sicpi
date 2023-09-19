#ifndef RUN_H
#define RUN_H

#include "object.h"
#include "source.h"

object load(source src);

object run(object program);

object load_run(source src);

#endif
