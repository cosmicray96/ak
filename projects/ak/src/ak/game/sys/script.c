#include "ak/game/sys/script.h"

//===== ak_sys_script =====//
//--- private ---//

static void
run_update(ak_sys_script* s, ak_dur delta)
{
}

//--- internal ---//
ak_sys_script
ak_sys_script_make(ak_wv* wv,
                   ak_wcb* wcb,
                   ak_alct alct)
{
  ak_sys_script s = { 0 };
  s.alct = alct;
  s.wv = wv;
  s.wcb = wcb;
  return s;
}

void
ak_sys_script_destroy(ak_sys_script* s);

void
ak_sys_script_update(ak_sys_script* s,
                     ak_dur delta)
{
}
