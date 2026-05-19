#ifndef ak_game_sys_script_h
#define ak_game_sys_script_h

#include "ak/coll/da.h"
#include "ak/coll/hmn.h"
#include "ak/core/async/thpool.h"
#include "ak/game/script/stg.h"
#include "ak/game/stg/ettgen.h"
#include "ak/game/world/cb.h"
#include "ak/game/world/view.h"
#include "ak/os/time.h"
typedef struct
{
  ak_alct alct;
  ak_wv* wv;
  ak_wcb* wcb;
  ak_ettgen* eg;
  ak_thpool* tp;

  ak_scriptstg* ss;
  ak_hmn map;
  ak_da depths;
} ak_sys_script;

ak_sys_script
ak_sys_script_make(ak_wv* wv,
                   ak_wcb* wcb,
                   ak_ettgen* eg,
                   ak_thpool* tp,
                   ak_alct alct);

void
ak_sys_script_destroy(ak_sys_script* s);

void
ak_sys_script_update(ak_sys_script* s,
                     ak_dur delta);

#endif
