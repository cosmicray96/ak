#ifndef ak_platform_platform_itn_h
#define ak_platform_platform_itn_h

#include "ak/app/eq.h"
#include "ak/platform/core.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <stdbool.h>

Display*
ak_platform_display_get(ak_platform* p);

void
ak_platform_window_regs(ak_platform* p,
                        ak_window* w);
void
ak_platform_window_unregs(ak_platform* p,
                          ak_window* w);

Window
ak_window_get(ak_window* w);
void
ak_window_clientmsg(ak_window* w,
                    ak_app_eq* eq,
                    XEvent* e);

bool
ak_window_key_pressed(ak_window* w,
                      ak_keycode kc);
void
ak_window_key_set(ak_window* w,
                  ak_keycode kc,
                  bool pressed);

ak_keycode
ak_keycode_from_x11(XKeyEvent* e);
#endif
