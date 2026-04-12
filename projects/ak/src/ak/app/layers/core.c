#include "ak/app/layers/core.h"
#include "ak/app/app.h"

//===== ak_lcore =====//
//--- private ---//
struct ak_lcore
{
  ak_alct alct;
  ak_app* app;
};

//--- public ---//
ak_lcore*
ak_lcore_make(ak_alct alct)
{
  ak_lcore* l =
    ak_alct_alloc(alct, sizeof(ak_lcore));
  return l;
}
void
ak_lcore_destroy(ak_lcore* l)
{
  ak_alct_free(l->alct, l);
}

ak_applayer
ak_lcore_to_applayer(ak_lcore* l)
{
  ak_applayer appl = { 0 };

  return appl;
}
