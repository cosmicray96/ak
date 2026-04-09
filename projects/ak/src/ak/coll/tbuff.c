#include "ak/coll/tbuff.h"

//===== ak_tbuff =====//

//--- public ---//
ak_tbuff
ak_tbuff_make(uint32_t itemsize,
              float gr,
              uint32_t sc,
              ak_alct alct)
{
  ak_tbuff t;
  t.dbuff =
    ak_dbuff_make(gr, itemsize * sc, alct);
  t.itemsize = itemsize;
  return t;
}

void
ak_tbuff_destroy(ak_tbuff* t)
{
  ak_dbuff_destroy(&t->dbuff);
  t->itemsize = 0;
}

void*
ak_tbuff_at(ak_tbuff* t, uint32_t idx)
{
  return ak_dbuff_at(&t->dbuff,
                     idx * t->itemsize);
}

void
ak_tbuff_grow(ak_tbuff* t)
{
  ak_dbuff_grow(&t->dbuff);
}

void
ak_tbuff_cap_inc(ak_tbuff* t,
                 uint32_t inc_by)
{
  ak_dbuff_cap_inc(&t->dbuff,
                   inc_by * t->itemsize);
}

void
ak_tbuff_grow_w_copy(ak_tbuff* t)
{
  ak_dbuff_grow_w_copy(&t->dbuff);
}

ak_ex void
ak_tbuff_cap_inc_w_copy(ak_tbuff* t,
                        uint32_t inc_by)
{
  ak_dbuff_cap_inc_w_copy(
    &t->dbuff, inc_by * t->itemsize);
}
