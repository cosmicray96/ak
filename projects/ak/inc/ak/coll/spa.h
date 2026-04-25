#ifndef ak_coll_spa_h
#define ak_coll_spa_h

#include "ak/coll/da.h"
#include "ak/coll/pbuff.h"
#include "ak/export.h"
#include <stdbool.h>
typedef struct ak_spa ak_spa;
struct ak_spa
{
  ak_pbuff s_to_d_idxs;
  ak_da denses;
  uint32_t itemsize;
};

ak_ex ak_spa
ak_spa_make(uint32_t itemsize, ak_alct alct);
ak_ex void
ak_spa_destroy(ak_spa* s);

ak_ex bool
ak_spa_exist(ak_spa* s, uint32_t s_idx);

ak_ex void*
ak_spa_at(ak_spa* s, uint32_t s_idx);

ak_ex void
ak_spa_insert(ak_spa* s,
              uint32_t s_idx,
              const void* item);
ak_ex void
ak_spa_overwrite(ak_spa* s,
                 uint32_t s_idx,
                 const void* item);

ak_ex void
ak_spa_remove(ak_spa* s, uint32_t s_idx);

#endif
