#ifndef ak_game_sys_tf_h
#define ak_game_sys_tf_h

#include "ak/coll/spa.h"
#include "ak/core/mem/allocator.h"
#include "ak/game/core.h"
#include "ak/game/world/cb.h"
#include "ak/game/world/view.h"
typedef struct
{
  ak_alct alct;
  ak_wv_itdfspre it;
  ak_spa gmat3s;
} ak_sys_tf;

ak_sys_tf
ak_sys_tf_make(ak_alct alct);
void
ak_sys_tf_destroy(ak_sys_tf* stf);

void
ak_sys_tf_update(ak_sys_tf* stf,
                 ak_wv* wv,
                 ak_wcb* wcb);

#endif
