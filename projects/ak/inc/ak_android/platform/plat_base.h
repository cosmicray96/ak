#ifndef ak_android_platform_plat_base_h
#define ak_android_platform_plat_base_h

#include "ak/platform/plat_base.h"

bool
ak_android_plat_base_surface_ready(
  ak_plat_base* pb);

void
ak_android_plat_base_rctx_loader_startup(
  ak_plat_base* pb);
void
ak_android_plat_base_rctx_loader_shutdown(
  ak_plat_base* pb);

#endif
