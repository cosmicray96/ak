#ifndef ak_game_sys_ren_h
#define ak_game_sys_ren_h

#include "ak/coll/da.h"
#include "ak/coll/hmn.h"
#include "ak/core/math/fixed.h"
#include "ak/core/mem/allocator.h"
#include "ak/game/world/view.h"
#include "ak/gfx/core.h"
#include "ak/gfx/gcb.h"
#include "ak/gfx/mtrl/stg.h"
#include "ak/os/time.h"

typedef struct
{
  ak_alct alct;
  ak_gfx* gf;
  ak_wv* wv;
  ak_mtrlstg* ms;
  ak_gcb* gcb;

  ak_hmn mtrls;
  ak_da free_mtrls;
  ak_fx time;
} ak_sys_ren;

ak_sys_ren
ak_sys_ren_make(ak_gfx* gf,
                ak_gcb* gcb,
                ak_mtrlstg* ms,
                ak_wv* wv,
                ak_alct alct);
void
ak_sys_ren_destroy(ak_sys_ren* r);

void
ak_sys_ren_render(ak_sys_ren* r,
                  ak_dur delta);

#endif
