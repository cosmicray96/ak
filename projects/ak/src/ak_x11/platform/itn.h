#ifndef ak_platform_plat_itn_h
#define ak_platform_plat_itn_h

#include "ak/coll/hmn.h"
#include "ak/coll/sla.h"
#include "ak/platform/core.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <stdbool.h>
#include <threads.h>

typedef struct
{
  Window wn;
  Atom wm_delete;
  bool key_down[ak_key_count];
} ak_win_itn;

struct ak_plat
{
  ak_sla win_itns;
  ak_hmn winmap;

  Display* d;
};

Display*
ak_plat_display(ak_plat* p);

bool
ak_window_key_pressed(ak_win_itn* win_itn,
                      ak_keycode kc);
void
ak_window_key_set(ak_win_itn* win_itn,
                  ak_keycode kc,
                  bool pressed);

ak_keycode
ak_keycode_from_x11(XKeyEvent* e);

#endif
