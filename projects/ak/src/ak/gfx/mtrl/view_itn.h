#ifndef ak_gfx_mtrl_view_itn_h
#define ak_gfx_mtrl_view_itn_h

#include "ak/gfx/mtrl/registry.h"
#include "ak/gfx/mtrl/view.h"

struct ak_mtrlstg_v
{
  ak_mtrlreg* mr;
};

ak_mtrlstg_v
ak_mtrlstg_v_make(ak_mtrlreg* mr);
void
ak_mtrlstg_v_destroy(ak_mtrlstg_v* v);

#endif
