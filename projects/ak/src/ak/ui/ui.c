#include "ak/ui/ui.h"
#include "ak/ui/core.h"

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
