#include "ak/platform/platform.h"
#include "ak/coll/hmn.h"
#include "ak/platform/core.h"
#include "ak_x11/platform/itn.h"

#include <X11/X.h>
#include <X11/Xlib.h>

//===== ak_platform =====//
//--- private ---//
struct ak_platform
{
  Display* d;
  ak_hmn windows;
};

typedef struct
{
  XEvent e;
  ak_window* w;
} item;

static bool
nextevent(ak_platform* p, item* itm)
{
  while (XPending(p->d)) {
    XNextEvent(p->d, &itm->e);

    Window win_cddt = itm->e.xany.window;

    if (!ak_hmn_exist_u64(&p->windows,
                          win_cddt)) {
      continue;
    }
    itm->w = *ak_hmn_at_u64_t(
      ak_window*, &p->windows, win_cddt);
    return true;
  }
  return false;
}

static bool
peekevent(ak_platform* p, item* itm)
{
  while (XPending(p->d)) {
    XPeekEvent(p->d, &itm->e);

    Window win_cddt = itm->e.xany.window;

    if (!ak_hmn_exist_u64(&p->windows,
                          win_cddt)) {
      XNextEvent(p->d, &itm->e);
      continue;
    }
    itm->w = *ak_hmn_at_u64_t(
      ak_window*, &p->windows, win_cddt);
    return true;
  }
  return false;
}

//--- public ---//
ak_platform*
ak_platform_startup(ak_alct alct)
{
  ak_platform* p =
    ak_alct_alloc(alct, sizeof(ak_platform));
  p->d = XOpenDisplay(NULL);
  p->windows =
    ak_hmn_make(sizeof(ak_window*), alct);
  return p;
}

void
ak_platform_shutdown(ak_platform* p)
{
  XCloseDisplay(p->d);
}

Display*
ak_platform_display_get(ak_platform* p)
{
  return p->d;
}

void
ak_platform_window_regs(ak_platform* p,
                        ak_window* w)
{
  Window win = ak_window_get(w);
  ak_hmn_insert_u64(&p->windows, win, &w);
}

void
ak_platform_window_unregs(ak_platform* p,
                          ak_window* w)
{

  Window win = ak_window_get(w);
  ak_hmn_remove_u64(&p->windows, win);
}

void
ak_platform_eventflush(ak_platform* p,
                       ak_app_eq* eq)
{

  ak_evt e = { 0 };
  e.type = ak_evt_type_win;
  item ex11 = { 0 };
  item nextx11 = { 0 };

  while (nextevent(p, &ex11)) {

    switch (ex11.e.type) {
      case ClientMessage: {
        ak_window_clientmsg(
          ex11.w, eq, &ex11.e);
        break;
      }

      case KeyPress: {
        e.win.type = ak_winevt_key;
        e.win.key.code =
          ak_keycode_from_x11(&ex11.e.xkey);
        e.win.key.action =
          ak_keyaction_pressed;
        e.win.key.mode = ak_keymode_none;

        if (e.win.key.code == ak_key_none) {
          continue;
        }

        if (!ak_window_key_pressed(
              ex11.w, e.win.key.code)) {
          ak_app_eq_push(eq, e);
        }
        ak_window_key_set(
          ex11.w, e.win.key.code, true);
        break;
      }

      case KeyRelease: {
        if (peekevent(p, &nextx11)) {
          if (nextx11.e.type == KeyPress &&
              nextx11.e.xkey.time ==
                ex11.e.xkey.time &&
              nextx11.e.xkey.keycode ==
                ex11.e.xkey.keycode) {
            // this release is from
            // autorepeat
            break;
          }
        }

        e.win.type = ak_winevt_key;
        e.win.key.code =
          ak_keycode_from_x11(&ex11.e.xkey);
        e.win.key.action =
          ak_keyaction_released;
        e.win.key.mode = ak_keymode_none;

        if (e.win.key.code == ak_key_none) {
          continue;
        }

        ak_app_eq_push(eq, e);
        ak_window_key_set(
          ex11.w, e.win.key.code, false);
        break;
      }
      default: {
        // no event
      }
    }
  }
}
