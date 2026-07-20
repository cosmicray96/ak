#include "ak/game/sys/ui.h"
#include "ak/ui/ui.h"

ak_sys_ui
ak_sys_ui_make(ak_alct alct)
{
  ak_sys_ui u = { 0 };
  u.ui = ak_ui_make(alct);
  u.map = ak_hmn_make(sizeof(ak_ett), alct);
  return u;
}
void
ak_sys_ui_destroy(ak_sys_ui* u)
{
  ak_ui_destroy(&u->ui);
}

void
ak_sys_ui_layout(ak_sys_ui* u, ak_wv* wv)
{
}

void
ak_sys_ui_event(ak_sys_ui* u,
                ak_wv* wv,
                ak_ett ett,
                const ak_evt* evt);

void
ak_sys_ui_render(ak_sys_ui* u,
                 ak_wv* wv,
                 ak_wcb* wcb);
