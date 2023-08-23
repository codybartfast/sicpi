#ifndef SICP_ERROR_H
#define SICP_ERROR_H




void alloc_error(const char *caller);
void eprintf(const char *msg, ...);
void eprintfx(const char *msg, ...);
void inyim(const char *msg, ...);

#endif
