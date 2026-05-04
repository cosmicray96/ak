#include "ak/gfx/mtrl/register.h"
#include "ak/coll/hmn.h"
#include "ak/debug.h"

//===== ak_mtrlreg =====//
//--- public ---//
ak_mtrlreg
ak_mtrlreg_make(ak_alct alct)
{
  ak_mtrlreg mr = { 0 };
  mr.map = ak_hmn_make(sizeof(void*), alct);
  return mr;
}
void
ak_mtrlreg_destroy(ak_mtrlreg* mr)
{
  ak_hmn_destroy(&mr->map);
}

void
ak_mtrlreg_insert(ak_mtrlreg* mr,
                  ak_mtrltype mt,
                  void* mtrl)
{
  ak_assert(!ak_hmn_exist_u64(&mr->map, mt));
  ak_hmn_insert_u64(&mr->map, mt, &mtrl);
}

void*
ak_mtrlreg_at(ak_mtrlreg* mr, ak_mtrltype mt)
{
  ak_assert(ak_hmn_exist_u64(&mr->map, mt));
  return *(void**)ak_hmn_at_u64(&mr->map,
                                mt);
}
