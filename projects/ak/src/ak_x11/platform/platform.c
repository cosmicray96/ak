#include "ak/platform/platform.h"
#include "ak/platform/core.h"

#include <X11/Xlib.h>

//===== ak_platform =====//
//--- private ---//
struct ak_platform
{
  Display* d;
};

//--- public ---//
ak_platform*
ak_platform_startup(ak_alct alct)
{
  ak_platform* p =
    ak_alct_alloc(alct, sizeof(ak_platform));
  p->d = XOpenDisplay(NULL);
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
