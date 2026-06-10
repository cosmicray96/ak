#ifndef ak_ui_ui_h
#define ak_ui_ui_h

#include "ak/coll/fcnst.h"
#include "ak/core/mem/allocator.h"

#include "ak/gfx/gcb.h"
#include "ak/gfx/mtrl.h"
#include "ak/ui/core.h"

typedef struct
{
  ak_cnst x_cnst;
  ak_cnst y_cnst;
  bool visible;
  bool clipping;
  ak_mtrl_quaddata qd;
} ak_uielm_args;

typedef struct
{
  ak_mtrl_basedata mtrl_b;
  ak_dq scissors;
} ak_uirstg;

typedef struct
{
  char _ph;
} ak_uilstg;

typedef struct
{
  ak_fcnst tree;
  bool setted;

  ak_uirstg rstg;
  ak_uilstg lstg;
} ak_ui;

ak_ui
ak_ui_make(ak_alct alct);
void
ak_ui_destroy(ak_ui* ui);

void
ak_ui_clear(ak_ui* ui);

ak_uiid
ak_ui_root(ak_ui* ui);

ak_uiid
ak_ui_add(ak_ui* ui,
          ak_uiid pt,
          const ak_uielm_args* args);

void
ak_ui_set(ak_ui* ui,
          float x,
          float y,
          float w,
          float h);

void
ak_ui_render(ak_ui* ui, ak_gcb* gcb);

#endif
