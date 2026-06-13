#ifndef ak_platform_plat_itn_h
#define ak_platform_plat_itn_h

#include "ak/platform/core.h"

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdbool.h>
#include <threads.h>

//--- inside ---//
ak_keycode
ak_keycode_from_x11(XKeyEvent* e);

#endif
