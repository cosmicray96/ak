#include "ak/platform/plat.h"
#include "ak/app/event.h"

#include "ak/platform/core.h"
#include "ak_x11/platform/itn.h"

#include <X11/X.h>
#include <X11/Xlib.h>

//===== ak_plat =====//
//--- private ---//
bool
ak_plat_visible(ak_plat* p)
{
  return p->visible;
}
void
ak_plat_visible_set(ak_plat* p, bool visible)
{
  p->visible = visible;
}
bool
ak_plat_key_pressed(ak_plat* p,
                    ak_keycode kc)
{
  return p->key_down[kc];
}

void
ak_plat_key_set(ak_plat* p,
                ak_keycode kc,
                bool pressed)
{
  p->key_down[kc] = pressed;
}

//--- public ---//
ak_plat*
ak_plat_startup(ak_alct alct)
{
  ak_plat* p =
    ak_alct_alloc(alct, sizeof(ak_plat));

  p->d = XOpenDisplay(NULL);
  p->screen = DefaultScreen(p->d);
  p->visible = true;

  uint32_t x = 10;
  uint32_t y = 10;
  uint32_t width = 200;
  uint32_t height = 150;
  uint32_t border = 1;

  p->wn = XCreateSimpleWindow(
    p->d,
    RootWindow(p->d, p->screen),
    x,
    y,
    width,
    height,
    border,
    BlackPixel(p->d, p->screen),
    WhitePixel(p->d, p->screen));

  XStoreName(p->d, p->wn, "Window Title!");

  p->wm_delete = XInternAtom(
    p->d, "WM_DELETE_WINDOW", False);
  XSetWMProtocols(
    p->d, p->wn, &p->wm_delete, 1);
  XSelectInput(
    p->d,
    p->wn,
    ButtonPressMask | ButtonReleaseMask |
      KeyPressMask | KeyReleaseMask |
      PointerMotionMask | FocusChangeMask |
      StructureNotifyMask |
      VisibilityChangeMask);

  XMapWindow(p->d, p->wn);
  XFlush(p->d);

  return p;
}

void
ak_plat_shutdown(ak_plat* p)
{
  XDestroyWindow(p->d, p->wn);
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
  XEvent ex11 = { 0 };
  XEvent nextx11 = { 0 };

  while (XPending(d)) {
    XNextEvent(d, &ex11);

    switch (ex11.type) {
      case ClientMessage: {
        if (p->wn != ex11.xany.window) {
          break;
        }

        if ((Atom)ex11.xclient.data.l[0] ==
            p->wm_delete) {
          e.win.type = ak_winevt_close;
          ak_app_eq_push(eq, e);
        }
        break;
      }

      case KeyPress: {
        if (p->wn != ex11.xkey.window) {
          break;
        }
        e.win.type = ak_winevt_key;
        e.win.key.code =
          ak_keycode_from_x11(&ex11.xkey);
        e.win.key.action =
          ak_keyaction_pressed;
        e.win.key.mode = ak_keymode_none;

        if (e.win.key.code == ak_key_none) {
          continue;
        }

        if (!ak_plat_key_pressed(
              p, e.win.key.code)) {
          ak_app_eq_push(eq, e);
        }
        ak_plat_key_set(
          p, e.win.key.code, true);
        break;
      }

      case KeyRelease: {
        if (p->wn != ex11.xkey.window) {
          break;
        }
        if (XPending(d)) {
          XPeekEvent(d, &nextx11);
          if (p->wn == nextx11.xkey.window) {

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
        e.win.key.code =
          ak_keycode_from_x11(&ex11.xkey);
        e.win.key.action =
          ak_keyaction_released;
        e.win.key.mode = ak_keymode_none;

        if (e.win.key.code == ak_key_none) {
          continue;
        }

        ak_app_eq_push(eq, e);
        ak_plat_key_set(
          p, e.win.key.code, false);
        break;
      }

      case FocusIn: {
        if (p->wn != ex11.xkey.window) {
          break;
        }
        e.win.type = ak_winevt_focus_gained;
        if (!ak_plat_visible(p)) {
          ak_app_eq_push(eq, e);
        }
        ak_plat_visible_set(p, true);
        break;
      }

      case FocusOut: {
        if (p->wn != ex11.xkey.window) {
          break;
        }
        e.win.type = ak_winevt_focus_lost;
        if (ak_plat_visible(p)) {
          ak_app_eq_push(eq, e);
        }
        ak_plat_visible_set(p, false);
        break;
      }

      case MapNotify: {
        if (p->wn != ex11.xkey.window) {
          break;
        }
        e.win.type = ak_winevt_visible;
        if (!ak_plat_visible(p)) {
          ak_app_eq_push(eq, e);
        }
        ak_plat_visible_set(p, true);
        break;
      }
      case UnmapNotify: {
        if (p->wn != ex11.xkey.window) {
          break;
        }
        e.win.type = ak_winevt_invisible;
        if (ak_plat_visible(p)) {
          ak_app_eq_push(eq, e);
        }
        ak_plat_visible_set(p, false);
        break;
      }

      case VisibilityNotify: {
        if (p->wn != ex11.xkey.window) {
          break;
        }
        if (ex11.xvisibility.state ==
            VisibilityFullyObscured) {
          e.win.type = ak_winevt_invisible;
          if (ak_plat_visible(p)) {
            ak_app_eq_push(eq, e);
          }
          ak_plat_visible_set(p, false);
        } else {
          e.win.type = ak_winevt_visible;
          if (!ak_plat_visible(p)) {
            ak_app_eq_push(eq, e);
          }
          ak_plat_visible_set(p, true);
        }
        break;
      }
      case ConfigureNotify: {
        e.win.type = ak_winevt_resize;

        e.win.resize.w =
          ex11.xconfigure.width;
        e.win.resize.h =
          ex11.xconfigure.height;
        ak_app_eq_push(eq, e);
        break;
      }
      default: {
        // no event
      }
    }
  }
}
