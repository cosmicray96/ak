#ifndef ak_ui_ui_dir_h
#define ak_ui_ui_dir_h

#include "ak/ui/core.h"
#include "ak/ui/ui.h"

typedef struct
{
  ak_cnst x_cnst, y_cnst;
  float x, y, w, h;
} ak_uielm;

void
ak_ui_layout(ak_ui* ui);

#endif
