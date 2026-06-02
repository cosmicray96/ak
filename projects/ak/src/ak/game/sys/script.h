#ifndef ak_game_sys_script_h
#define ak_game_sys_script_h

#include "ak/coll/da.h"
#include "ak/game/core.h"
#include "ak/game/script/stg.h"
#include "ak/os/time.h"

typedef struct
{
  ak_alct alct;
  ak_scriptstg* ss;

  ak_wv* wv;

  ak_da inits;
  ak_da deinits;

} ak_sys_script;

ak_sys_script
ak_sys_script_make(ak_alct alct);
void
ak_sys_script_destroy(ak_sys_script* s);

void
ak_sys_script_set(ak_sys_script* s,
                  ak_wv* wv,
                  ak_wcb* wcb);

void
ak_sys_script_run_init(ak_sys_script* s,
                       ak_wcb* output_wcb);
void
ak_sys_script_run_deinit(ak_sys_script* s,
                         ak_wcb* output_wcb);
void
ak_sys_script_run_event(ak_sys_script* s,
                        ak_evt e,
                        ak_wcb* output_wcb);
void
ak_sys_script_run_update(ak_sys_script* s,
                         ak_dur delta,
                         ak_wcb* output_wcb);

void
ak_sys_script_run_shutdown(
  ak_sys_script* s,
  ak_wcb* output_wcb);

#endif
