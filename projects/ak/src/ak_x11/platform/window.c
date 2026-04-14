#include "ak/platform/window.h"
#include "ak/app/eq.h"
#include "ak/app/event.h"
#include "ak/debug.h"
#include "ak/platform/core.h"
#include "ak_x11/platform/itn.h"

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

//===== ak_window =====//
//--- private ---//

struct ak_window
{
  ak_platform* p;
  Display* dpy;
  int screen;
  Window win;
  Atom wm_delete;

  bool key_down[ak_key_count];
};

Window
ak_window_get(ak_window* w)
{
  return w->win;
}

//--- public ---//
ak_window*
ak_window_make(ak_platform* p, ak_alct alct)
{
  ak_window* w =
    ak_alct_alloc(alct, sizeof(ak_window));
  w->p = p;

  for (uint32_t i = 0; i < ak_key_count;
       i++) {
    w->key_down[i] = false;
  }

  w->dpy = ak_platform_display_get(p);
  w->screen = DefaultScreen(w->dpy);

  w->win = XCreateSimpleWindow(
    w->dpy,
    RootWindow(w->dpy, w->screen),
    100,
    100,
    800,
    600,
    1,
    BlackPixel(w->dpy, w->screen),
    WhitePixel(w->dpy, w->screen));

  XSizeHints hints = { 0 };
  hints.flags = PPosition | PSize;
  hints.x = 100;
  hints.y = 100;
  hints.width = 800;
  hints.height = 600;
  XSetNormalHints(w->dpy, w->win, &hints);

  XStoreName(w->dpy,
             w->win,
             "Triangle Linear Color Blend");

  w->wm_delete = XInternAtom(
    w->dpy, "WM_DELETE_WINDOW", False);
  XSetWMProtocols(
    w->dpy, w->win, &w->wm_delete, 1);
  XSelectInput(
    w->dpy,
    w->win,
    ButtonPressMask | ButtonReleaseMask |
      KeyPressMask | KeyReleaseMask |
      FocusChangeMask | StructureNotifyMask);

  XMapWindow(w->dpy, w->win);
  ak_platform_window_regs(p, w);

  return w;
}

void
ak_window_destroy(ak_window* w)
{
  XDestroyWindow(w->dpy, w->win);
  ak_platform_window_unregs(w->p, w);
}

void
ak_window_clientmsg(ak_window* w,
                    ak_app_eq* eq,
                    XEvent* ex11)
{
  ak_evt e = { 0 };
  e.type = ak_evt_type_win;
  e.win.win = w;

  if (ex11->type != ClientMessage) {
    return;
  }

  if ((Atom)ex11->xclient.data.l[0] ==
      w->wm_delete) {
    e.win.type = ak_winevt_close;
    ak_app_eq_push(eq, e);
  }
}

bool
ak_window_key_pressed(ak_window* w,
                      ak_keycode kc)
{
  ak_assert(kc < ak_key_count);
  return w->key_down[kc];
}
void
ak_window_key_set(ak_window* w,
                  ak_keycode kc,
                  bool pressed)
{
  ak_assert(kc < ak_key_count);
  w->key_down[kc] = pressed;
}
/*
void
ak_window_eventflush(ak_window* w,
                     ak_app_eq* eq)
{
  ak_evt e = { 0 };
  e.type = ak_evt_type_win;
  e.win.win = w;
  XEvent ex11;
  XEvent nextx11;

  while (XPending(w->dpy)) {
    XNextEvent(w->dpy, &ex11);

    if (ex11.xany.window != w->win) {
      continue;
    }

    switch (ex11.type) {
      case ClientMessage: {
        if ((Atom)ex11.xclient.data.l[0] ==
            w->wm_delete) {
          e.win.type = ak_winevt_close;
          ak_app_eq_push(eq, e);
        }
        break;
      }

      case KeyPress: {
        e.win.type = ak_winevt_key;

        e.win.key.code =
          ak_keycode_from_x11(&ex11.xkey);
        e.win.key.action =
          ak_keyaction_pressed;
        e.win.key.mode = ak_keymode_none;

        if (e.win.key.code == ak_key_none) {
          continue;
        }

        if (!w->key_down[e.win.key.code]) {
          ak_app_eq_push(eq, e);
        }
        w->key_down[e.win.key.code] = true;

        break;
      }

      case KeyRelease: {

        if (XPending(w->dpy)) {
          XPeekEvent(w->dpy, &nextx11);
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

        e.win.type = ak_winevt_key;

        e.win.key.code =
          ak_keycode_from_x11(&ex11.xkey);
        e.win.key.action =
          ak_keyaction_released;
        e.win.key.mode = ak_keymode_none;

        if (e.win.key.code == ak_key_none) {
          continue;
        }

        ak_app_eq_push(eq, e);
        w->key_down[e.win.key.code] = false;
        break;
      }
      default: {
        // no event
      }
    }
  }
}
*/
