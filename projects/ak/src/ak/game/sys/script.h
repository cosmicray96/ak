#ifndef ak_game_sys_script_h
#define ak_game_sys_script_h

#include "ak/coll/da.h"
#include "ak/game/core.h"
#include "ak/game/script/stg.h"
#include "ak/game/stg/world.h"
#include "ak/game/world/cb_itn.h"
#include "ak/os/time.h"
#include "ak/system/idgen.h"

typedef struct
{
  ak_alct alct;
  ak_scriptstg* ss;

  ak_wcb wcb;
  ak_wcb wcb_output;

  ak_da inits;
  ak_da deinits;

} ak_sys_script;

ak_sys_script
ak_sys_script_make(ak_idgen* ig,
                   ak_alct alct);
void
ak_sys_script_destroy(ak_sys_script* s);

void
ak_sys_script_destroy_world(ak_sys_script* s,
                            ak_world* w);

void
ak_sys_script_on_event(ak_sys_script* s,
                       ak_world* w,
                       ak_ett ett,
                       const ak_evt* evt);
void
ak_sys_script_update(ak_sys_script* s,
                     ak_world* w);

void
ak_sys_script_wcb_apply(ak_sys_script* s,
                        ak_world* w,
                        ak_wcb* wcb);

#endif
