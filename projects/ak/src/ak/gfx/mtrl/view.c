#include "ak/gfx/mtrl/view.h"
#include "ak/gfx/mtrl/registry.h"
#include "ak/gfx/mtrl/view_itn.h"

//===== ak_mtrlstg_v =====//
//--- internal ---//
ak_mtrlstg_v
ak_mtrlstg_v_make(ak_mtrlreg* mr)
{
  ak_mtrlstg_v v = { 0 };
  v.mr = mr;
  return v;
}

void
ak_mtrlstg_v_destroy(ak_mtrlstg_v* v)
{
  v->mr = 0;
}

//--- export ---//
uint32_t
ak_mtrlstg_v_new(ak_mtrlstg_v* v,
                 ak_mtrl_enum me)
{
  return ak_mtrlreg_new(v->mr, me);
}

void
ak_mtrlstg_v_remove(ak_mtrlstg_v* v,
                    ak_mtrl_enum me,
                    uint32_t idx)
{
  ak_mtrlreg_remove(v->mr, me, idx);
}

void*
ak_mtrlstg_v_at(ak_mtrlstg_v* v,
                ak_mtrl_enum me,
                uint32_t idx)
{
  return ak_mtrlreg_at(v->mr, me, idx);
}
