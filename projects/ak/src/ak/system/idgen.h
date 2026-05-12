#ifndef ak_system_idgen_h
#define ak_system_idgen_h

#include "ak/coll/dq.h"
#include <stdint.h>
typedef struct
{
  ak_dq freelist;
  uint32_t counter;
} ak_idgen;

ak_idgen
ak_idgen_make(ak_alct alct);
void
ak_idgen_destroy(ak_idgen* ig);

uint32_t
ak_idgen_new(ak_idgen* ig);
void
ak_idgen_remove(ak_idgen* ig, uint32_t id);

#endif
