#ifndef ak_app_impls_simple_h
#define ak_app_impls_simple_h

#include <stdbool.h>
typedef struct ak_lsimple ak_lsimple;

ak_lsimple*
ak_lsimple_make();

void
ak_lsimple_destroy(ak_lsimple* l);

bool
ak_lsimple_tick(void* l);

#endif
