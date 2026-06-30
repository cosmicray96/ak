#include "ak_x11/platform/plat.h"
#include "ak/app/event.h"

#include "ak/core/mem/ptr.h"
#include "ak/platform/core.h"

#include "ak_x11/platform/impl.h"
#include "ak_x11/platform/plat.h"

#include <X11/X.h>
#include <X11/Xlib.h>

//===== ak_plat =====//
//--- private ---//

struct ak_plat
{
  ak_alct alct;
  Display* d;
  Window wn;
  Atom wm_delete;
  uint32_t width;
  uint32_t height;
  bool key_down[ak_key_count];
  bool visible;
};

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
ak_plat_startup(Display* d,
                Window wn,
                Atom wm_delete,
                uint32_t init_width,
                uint32_t init_height,
                ak_alct alct)
{
  ak_plat* p =
    ak_alct_alloc(alct, sizeof(ak_plat));
  p->alct = alct;

  p->d = d;
  p->wn = wn;
  p->wm_delete = wm_delete;

  p->width = init_width;
  p->height = init_height;

  ak_p_set_byte(
    p->key_down, false, ak_key_count);
  p->visible = true;

  return p;
}

void
ak_plat_shutdown(ak_plat* p)
{
  ak_alct_free(p->alct, p);
}

int32_t
ak_plat_width(ak_plat* p)
{
  return p->width;
}
int32_t
ak_plat_height(ak_plat* p)
{
  return p->height;
}

void
ak_plat_eventflush(ak_plat* p, ak_app_eq* eq)
{
  ak_evt e = { 0 };
  e.type = ak_evttype_win;
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
          e.win.type = ak_evtwintype_close;
          ak_app_eq_push(eq, &e);
        }
        break;
      }

      case KeyPress: {
        if (p->wn != ex11.xkey.window) {
          break;
        }
        e.win.type = ak_evtwintpye_key;
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
          ak_app_eq_push(eq, &e);
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

        e.win.type = ak_evtwintpye_key;
        e.win.key.code =
          ak_keycode_from_x11(&ex11.xkey);
        e.win.key.action =
          ak_keyaction_released;
        e.win.key.mode = ak_keymode_none;

        if (e.win.key.code == ak_key_none) {
          continue;
        }

        ak_app_eq_push(eq, &e);
        ak_plat_key_set(
          p, e.win.key.code, false);
        break;
      }

      case FocusIn: {
        if (p->wn != ex11.xkey.window) {
          break;
        }
        e.win.type =
          ak_evtwintype_focus_gained;
        if (!ak_plat_visible(p)) {
          ak_app_eq_push(eq, &e);
        }
        ak_plat_visible_set(p, true);
        break;
      }

      case FocusOut: {
        if (p->wn != ex11.xkey.window) {
          break;
        }
        e.win.type =
          ak_evtwintype_focus_lost;
        if (ak_plat_visible(p)) {
          ak_app_eq_push(eq, &e);
        }
        ak_plat_visible_set(p, false);
        break;
      }

      case MapNotify: {
        if (p->wn != ex11.xkey.window) {
          break;
        }
        e.win.type = ak_evtwintype_visible;
        if (!ak_plat_visible(p)) {
          ak_app_eq_push(eq, &e);
        }
        ak_plat_visible_set(p, true);
        break;
      }
      case UnmapNotify: {
        if (p->wn != ex11.xkey.window) {
          break;
        }
        e.win.type = ak_evtwintype_invisible;
        if (ak_plat_visible(p)) {
          ak_app_eq_push(eq, &e);
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
          e.win.type =
            ak_evtwintype_invisible;
          if (ak_plat_visible(p)) {
            ak_app_eq_push(eq, &e);
          }
          ak_plat_visible_set(p, false);
        } else {
          e.win.type = ak_evtwintype_visible;
          if (!ak_plat_visible(p)) {
            ak_app_eq_push(eq, &e);
          }
          ak_plat_visible_set(p, true);
        }
        break;
      }
      case ConfigureNotify: {
        e.win.type = ak_evtwintype_resize;
        XWindowAttributes attrs;
        XGetWindowAttributes(
          p->d,
          ex11.xconfigure.window,
          &attrs);

        int x11_width = attrs.width;
        int x11_height = attrs.height;

        p->width =
          x11_width > 0 ? x11_width : 1;
        p->height =
          x11_height > 0 ? x11_height : 1;

        e.win.type = ak_evtwintype_resize;
        e.win.resize.w = p->width;
        e.win.resize.h = p->height;
        ak_app_eq_push(eq, &e);
        break;
      }
      default: {
        // no event
      }
    }
  }
}
