#ifndef ak_coll_tbuff_h
#define ak_coll_tbuff_h

#include "ak/coll/dbuff.h"
typedef struct ak_tbuff ak_tbuff;
struct ak_tbuff
{
  ak_dbuff dbuff;
  uint32_t itemsize;
};

ak_ex ak_tbuff
ak_tbuff_make(uint32_t itemsize,
              float gr,
              uint32_t sc,
              ak_alct alct);
ak_ex void
ak_tbuff_destroy(ak_tbuff* t);

ak_ex void*
ak_tbuff_at(ak_tbuff* t, uint32_t idx);

ak_ex void
ak_tbuff_grow(ak_tbuff* t);
ak_ex void
ak_tbuff_cap_inc(ak_tbuff* t,
                 uint32_t inc_by);

ak_ex void
ak_tbuff_grow_w_copy(ak_tbuff* t);
ak_ex void
ak_tbuff_cap_inc_w_copy(ak_tbuff* t,
                        uint32_t inc_by);

#endif
