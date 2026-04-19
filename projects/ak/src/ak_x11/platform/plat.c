#include "ak/platform/plat.h"
#include "ak/app/event.h"
#include "ak/coll/hmn.h"
#include "ak/coll/sla.h"

#include "ak/platform/core.h"
#include "ak_x11/platform/itn.h"

#include <X11/X.h>
#include <X11/Xlib.h>

//===== ak_plat =====//
static bool
win_itn_get(ak_plat* p,
            Window wn,
            ak_window* w,
            ak_win_itn** win_itn)
{
  if (!ak_hmn_at_u64(&p->winmap, wn)) {
    return false;
  }

  *w = *(ak_window*)ak_hmn_at_u64(&p->winmap,
                                  wn);

  if (!ak_sla_at(&p->win_itns, *w)) {
    return false;
  }

  *win_itn = ak_sla_at(&p->win_itns, *w);
  return true;
}

//--- public ---//
ak_plat*
ak_plat_startup(ak_alct alct)
{
  ak_plat* p =
    ak_alct_alloc(alct, sizeof(ak_plat));
  p->d = XOpenDisplay(NULL);
  p->win_itns =
    ak_sla_make(sizeof(ak_window), alct);
  p->winmap =
    ak_hmn_make(sizeof(ak_sla_h), alct);
  return p;
}

void
ak_plat_shutdown(ak_plat* p)
{

  ak_sla_destroy(&p->win_itns);
  ak_hmn_destroy(&p->winmap);
  XCloseDisplay(p->d);
}

Display*
ak_plat_display(ak_plat* p)
{
  return p->d;
}

void
ak_plat_eventflush(ak_plat* p, ak_app_eq* eq)
{
  ak_evt e = { 0 };
  e.type = ak_evt_type_win;
  Display* d = p->d;
  ak_window w = { 0 };
  ak_win_itn* win_itn = 0;
  XEvent ex11 = { 0 };
  XEvent nextx11 = { 0 };

  while (XPending(d)) {
    XNextEvent(d, &ex11);

    switch (ex11.type) {
      case ClientMessage: {
        if (!win_itn_get(p,
                         ex11.xany.window,
                         &w,
                         &win_itn)) {
          break;
        }

        if ((Atom)ex11.xclient.data.l[0] ==
            win_itn->wm_delete) {
          e.win.type = ak_winevt_close;
          ak_app_eq_push(eq, e);
        }
        break;
      }

      case KeyPress: {
        if (!win_itn_get(p,
                         ex11.xkey.window,
                         &w,
                         &win_itn)) {
          break;
        }
        e.win.type = ak_winevt_key;
        e.win.w = w;
        e.win.key.code =
          ak_keycode_from_x11(&ex11.xkey);
        e.win.key.action =
          ak_keyaction_pressed;
        e.win.key.mode = ak_keymode_none;

        if (e.win.key.code == ak_key_none) {
          continue;
        }

        if (!ak_window_key_pressed(
              win_itn, e.win.key.code)) {
          ak_app_eq_push(eq, e);
        }
        ak_window_key_set(
          win_itn, e.win.key.code, true);
        break;
      }

      case KeyRelease: {
        if (XPending(d)) {
          XPeekEvent(d, &nextx11);
          if (win_itn_get(p,
                          ex11.xkey.window,
                          &w,
                          &win_itn)) {
            if (nextx11.type == KeyPress &&
                nextx11.xkey.time ==
                  ex11.xkey.time &&
                nextx11.xkey.keycode ==
                  ex11.xkey.keycode) {
              // this release is from
              // autorepeat
              break;
            }
          }
        }

        e.win.type = ak_winevt_key;
        e.win.w = w;
        e.win.key.code =
          ak_keycode_from_x11(&ex11.xkey);
        e.win.key.action =
          ak_keyaction_released;
        e.win.key.mode = ak_keymode_none;

        if (e.win.key.code == ak_key_none) {
          continue;
        }

        ak_app_eq_push(eq, e);
        ak_window_key_set(
          win_itn, e.win.key.code, false);
        break;
      }
      default: {
        // no event
      }
    }
  }
}
