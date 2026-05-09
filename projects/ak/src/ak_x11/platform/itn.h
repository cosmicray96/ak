#ifndef ak_platform_plat_itn_h
#define ak_platform_plat_itn_h

#include "ak/platform/core.h"
#include "ak/platform/plat_base.h"

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdbool.h>
#include <threads.h>

//--- inside ---//
ak_keycode
ak_keycode_from_x11(XKeyEvent* e);

//--- outside ---//
Display*
ak_plat_ren_display(ak_plat_base* pr);
Window
ak_plat_ren_window(ak_plat_base* pr);
Atom
ak_plat_ren_wm_delete(ak_plat_base* pr);

#endif
