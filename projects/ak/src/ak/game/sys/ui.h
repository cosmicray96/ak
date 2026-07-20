#ifndef ak_game_sys_ui_h
#define ak_game_sys_ui_h

#include "ak/app/event.h"
#include "ak/coll/hmn.h"
#include "ak/core/mem/allocator.h"
#include "ak/game/core.h"
#include "ak/ui/ui.h"

typedef struct
{
  ak_ui ui;
  ak_hmn map;
} ak_sys_ui;

ak_sys_ui
ak_sys_ui_make(ak_alct alct);
void
ak_sys_ui_destroy(ak_sys_ui* u);

void
ak_sys_ui_layout(ak_sys_ui* u, ak_wv* wv);

void
ak_sys_ui_event(ak_sys_ui* u,
                ak_wv* wv,
                ak_ett ett,
                const ak_evt* evt);

void
ak_sys_ui_render(ak_sys_ui* u,
                 ak_wv* wv,
                 ak_wcb* wcb);

#endif
