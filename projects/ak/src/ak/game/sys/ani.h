#ifndef ak_game_sys_ani_h
#define ak_game_sys_ani_h

#include "ak/core/mem/allocator.h"
#include "ak/game/core.h"
#include "ak/os/time.h"
#include "ak/res/reg.h"

typedef struct
{
} ak_sys_ani;

ak_sys_ani
ak_sys_ani_make(ak_alct alct);
void
ak_sys_ani_destroy(ak_sys_ani* sani);

void
ak_sys_ani_update(ak_sys_ani* sa,
                  ak_wv* wv,
                  ak_wcb* wcb,
                  ak_resreg* rr,
                  ak_dur delta);

#endif
