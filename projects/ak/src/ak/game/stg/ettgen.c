#include "ak/game/stg/ettgen.h"
#include "ak/coll/dq.h"

//===== ak_ettgen =====//
//--- public ---//
ak_ettgen
ak_ettgen_make(ak_alct alct)
{
  ak_ettgen eg = { 0 };
  eg.freelist =
    ak_dq_make(sizeof(ak_ett), 10, alct);
  eg.counter = 1;

  return eg;
}

void
ak_ettgen_destroy(ak_ettgen* eg)
{
  ak_dq_destroy(&eg->freelist);
  eg->counter = 0;
}

ak_ett
ak_ettgen_new(ak_ettgen* eg)
{
  ak_ett e = 0;
  if (ak_dq_pop(&eg->freelist, &e)) {
    return e;
  }

  e = eg->counter;
  eg->counter++;
  return e;
}

void
ak_ettgen_remove(ak_ettgen* eg, ak_ett e)
{
  // :(
}

bool
ak_ettgen_exist(ak_ettgen* eg, ak_ett e);
