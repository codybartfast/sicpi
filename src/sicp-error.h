#ifndef SICPERROR_H
#define SICPERROR_H

#include <inttypes.h>

void alloc_error(char *caller);
void eprintf(const char *msg, ...);
void eprintfx(const char *msg, ...);
void inyim(const char *msg, ...);

#endif
