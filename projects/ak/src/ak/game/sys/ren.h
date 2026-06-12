#ifndef ak_game_sys_ren_h
#define ak_game_sys_ren_h

#include "ak/coll/da.h"
#include "ak/coll/hmn.h"
#include "ak/core/math/fixed.h"
#include "ak/core/mem/allocator.h"
#include "ak/game/core.h"
#include "ak/game/world/view.h"
#include "ak/gfx/core.h"
#include "ak/gfx/gcb.h"
#include "ak/os/time.h"

typedef struct
{
  ak_alct alct;

  ak_hmn mtrls;
  ak_da free_mtrls;
  ak_fx time;
} ak_sys_ren;

ak_sys_ren
ak_sys_ren_make(ak_alct alct);
void
ak_sys_ren_destroy(ak_sys_ren* r);

void
ak_sys_ren_render(ak_sys_ren* r,
                  ak_wv* wv,
                  ak_gcb* gcb,
                  ak_dur delta);

#endif
