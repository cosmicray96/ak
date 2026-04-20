#ifndef ak_platform_plat_itn_h
#define ak_platform_plat_itn_h

#include "ak/platform/core.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <stdbool.h>
#include <threads.h>

struct ak_plat
{
  Display* d;
  Window wn;
  Atom wm_delete;
  int screen;
  bool key_down[ak_key_count];
  bool visible;
};

ak_keycode
ak_keycode_from_x11(XKeyEvent* e);

#endif
