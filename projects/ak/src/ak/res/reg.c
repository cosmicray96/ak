#include "ak/res/reg.h"
#include "ak/coll/hmn.h"
#include "ak/core/async/mutex.h"
#include "ak/debug.h"
#include "ak/res/core.h"
#include "ak/res/reg_itn.h"

//--- internal ---//
ak_resreg
ak_resreg_make(ak_alct alct)
{
  return (
    ak_resreg){ .m = ak_mutex_make(),
                .reses = ak_hmn_make(
                  sizeof(ak_res), alct) };
}
void
ak_resreg_destroy(ak_resreg* rr)
{
  ak_mutex_destroy(&rr->m);
  ak_hmn_destroy(&rr->reses);
}

//--- export ---//
ak_res
ak_resreg_get(ak_resreg* rr, ak_resid id)
{
  ak_mutex_lock(&rr->m);
  if (!ak_hmn_exist(&rr->reses, id)) {
    return (ak_res){ 0 };
  }
  ak_res r =
    *(ak_res*)ak_hmn_at(&rr->reses, id);
  ak_mutex_unlock(&rr->m);
  return r;
}

void
ak_resreg_add(ak_resreg* rr,
              ak_resid id,
              ak_res res)
{
  ak_mutex_lock(&rr->m);
  ak_assert(!ak_hmn_exist(&rr->reses, id));
  ak_hmn_insert(&rr->reses, id, &res);
  ak_mutex_unlock(&rr->m);
}

void
ak_resreg_remove(ak_resreg* rr, ak_resid id)
{
  ak_mutex_lock(&rr->m);
  ak_assert(ak_hmn_exist(&rr->reses, id));
  ak_hmn_remove(&rr->reses, id);
  ak_mutex_unlock(&rr->m);
}
