#ifndef ak_ui_ui_h
#define ak_ui_ui_h

#include "ak/coll/fcnst.h"
#include "ak/core/mem/allocator.h"

#include "ak/gfx/gcb.h"
#include "ak/gfx/mtrl.h"
#include "ak/ui/core.h"

typedef struct
{
  ak_mtrl_basedata mtrl_b;
  ak_dq scissors;
} ak_uirstg;

typedef struct
{
  ak_fcnst tree;
  ak_uirstg rstg;
} ak_ui;

ak_ui
ak_ui_make(ak_alct alct);
void
ak_ui_destroy(ak_ui* ui);

ak_uiid
ak_ui_add(ak_ui* ui,
          ak_uiid pt,
          ak_cnst x_cnst,
          ak_cnst y_cnst);

void
ak_ui_clear(ak_ui* ui);

void
ak_ui_render(ak_ui* ui, ak_gcb* gcb);

#endif
