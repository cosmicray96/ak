#ifndef ak_game_sys_script_h
#define ak_game_sys_script_h

#include "ak/coll/hmn.h"
#include "ak/game/world/cb.h"
#include "ak/game/world/view.h"
#include "ak/os/time.h"
typedef struct
{
  ak_wv* wv;
  ak_wcb* wcb;
  ak_alct alct;
  ak_hmn map;
} ak_sys_script;

ak_sys_script
ak_sys_script_make(ak_wv* wv,
                   ak_wcb* wcb,
                   ak_alct alct);

void
ak_sys_script_destroy(ak_sys_script* s);

void
ak_sys_script_update(ak_sys_script* s,
                     ak_dur delta);

#endif
