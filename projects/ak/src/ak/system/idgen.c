#include "ak/system/idgen.h"
#include "ak/coll/dq.h"
#include <stdint.h>

//===== ak_idgen =====//
//--- public ---//
ak_idgen
ak_idgen_make(ak_alct alct)
{
  ak_idgen ig = { 0 };
  ig.freelist =
    ak_dq_make(sizeof(uint32_t), alct);
  ig.counter = 1;

  return ig;
}

void
ak_idgen_destroy(ak_idgen* ig)
{
  ak_dq_destroy(&ig->freelist);
  ig->counter = 0;
}

uint32_t
ak_idgen_new(ak_idgen* ig)
{
  uint32_t id = 0;
  if (ak_dq_pop(&ig->freelist, &id)) {
    return id;
  }

  id = ig->counter;
  ig->counter++;
  return id;
}

void
ak_idgen_remove(ak_idgen* ig, uint32_t id)
{
  ak_dq_push(&ig->freelist, &id);
}
