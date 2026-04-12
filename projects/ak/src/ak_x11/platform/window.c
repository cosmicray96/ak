#include "ak/platform/window.h"
#include "ak/app/event.h"
#include "ak/platform/core.h"
#include "ak_x11/platform/platform_itn.h"
#include <X11/X.h>
#include <X11/Xlib.h>

//===== ak_window =====//
//--- private ---//
struct ak_window
{
  Display* dpy;
  int screen;
  Window win;
  Atom wm_delete;
};

ak_window*
ak_window_make(ak_platform* p, ak_alct alct)
{
  ak_window* w =
    ak_alct_alloc(alct, sizeof(ak_window));

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

  XMapWindow(w->dpy, w->win);
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
      FocusChangeMask | KeyPressMask |
      StructureNotifyMask);

  return w;
}

void
ak_window_destroy(ak_window* w)
{
  XDestroyWindow(w->dpy, w->win);
}

void
ak_window_eventflush(ak_window* w,
                     ak_app_eq* eq)
{
  ak_evt appevt = { 0 };
  appevt.type = ak_evt_type_win;
  appevt.win.win = w;
  XEvent ev;

  while (XPending(w->dpy)) {

    XNextEvent(w->dpy, &ev);
    switch (ev.type) {

      case ClientMessage:
        if ((Atom)ev.xclient.data.l[0] ==
            w->wm_delete) {
          appevt.win.type = ak_winevt_close;
        }
        break;
    }
  }
}
