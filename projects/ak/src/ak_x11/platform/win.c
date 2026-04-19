#include "ak/platform/plat.h"

#include "ak_x11/platform/itn.h"

ak_window
ak_plat_win_make(ak_plat* p)
{
  ak_win_itn win_itn = { 0 };

  Display* d = ak_plat_display(p);
  int screen = DefaultScreen(d);

  uint32_t x = 10;
  uint32_t y = 10;
  uint32_t width = 200;
  uint32_t height = 150;
  uint32_t border = 1;

  win_itn.wn = XCreateSimpleWindow(
    d,
    RootWindow(d, screen),
    x,
    y,
    width,
    height,
    border,
    BlackPixel(d, screen),
    WhitePixel(d, screen));

  XStoreName(d, win_itn.wn, "Window Title!");

  win_itn.wm_delete = XInternAtom(
    d, "WM_DELETE_WINDOW", False);
  XSetWMProtocols(
    d, win_itn.wn, &win_itn.wm_delete, 1);
  XSelectInput(
    d,
    win_itn.wn,
    ButtonPressMask | ButtonReleaseMask |
      KeyPressMask | KeyReleaseMask |
      FocusChangeMask | StructureNotifyMask);

  XMapWindow(d, win_itn.wn);
  XFlush(d);

  ak_window w =
    ak_sla_insert(&p->win_itns, &win_itn);
  ak_hmn_insert_u64(
    &p->winmap, win_itn.wn, &w);
  return w;
}

void
ak_plat_win_destroy(ak_plat* p, ak_window w)
{
  ak_win_itn* win_itn =
    ak_sla_at(&p->win_itns, w);

  XDestroyWindow(ak_plat_display(p),
                 win_itn->wn);
}

bool
ak_window_key_pressed(ak_win_itn* win_itn,
                      ak_keycode kc)
{
  return win_itn->key_down[kc];
}

void
ak_window_key_set(ak_win_itn* win_itn,
                  ak_keycode kc,
                  bool pressed)
{
  win_itn->key_down[kc] = pressed;
}
