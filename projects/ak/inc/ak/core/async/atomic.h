#ifndef ak_core_async_atmoic_h
#define ak_core_async_atmoic_h

#include "ak/export.h"

#include <stdint.h>

typedef struct
{
  int32_t v;
} ak_atomicint;

ak_ex ak_atomicint
ak_atomicint_make(int32_t v);

ak_ex void
ak_atomicint_store(ak_atomicint* ai,
                   int32_t v);
ak_ex int32_t
ak_atomicint_load(ak_atomicint* ai);

#endif
