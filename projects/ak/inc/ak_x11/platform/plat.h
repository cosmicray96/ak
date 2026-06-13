#ifndef ak_x11_platforn_plat_h
#define ak_x11_platforn_plat_h

#include "ak/export.h"
#include "ak/platform/plat_base.h"
#include <X11/Xlib.h>

ak_ex Display*
ak_plat_base_display(ak_plat_base* pr);

ak_ex Window
ak_plat_base_window(ak_plat_base* pr);

ak_ex Atom
ak_plat_base_wm_delete(ak_plat_base* pr);

#endif
