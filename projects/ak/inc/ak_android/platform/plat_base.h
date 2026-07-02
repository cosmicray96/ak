#ifndef ak_android_platform_plat_base_h
#define ak_android_platform_plat_base_h

#include "ak/platform/plat_base.h"

bool
ak_android_plat_base_render_trylock(
  ak_plat_base* pb);
void
ak_android_plat_base_render_lock(
  ak_plat_base* pb);
void
ak_android_plat_base_render_unlock(
  ak_plat_base* pb);

#endif
