#include "ak/platform/plat_base.h"

ak_plat_base*
ak_plat_base_startup(ak_alct alct)
{
  return 0;
}

void
ak_plat_base_shutdown(ak_plat_base* pb)
{
}

void
ak_plat_base_swapbuffer(ak_plat_base* pb)
{
}

void
ak_plat_base_eventflush(ak_plat_base* pb,
                        ak_app_eq* eq)
{
}

uint32_t
ak_plat_base_width(ak_plat_base* pb)
{
  return 0;
}
uint32_t
ak_plat_base_height(ak_plat_base* pb)
{
  return 0;
}

void
ak_plat_base_render_lock(ak_plat_base* pb)
{
}
void
ak_plat_base_render_unlock(ak_plat_base* pb)
{
}

void
ak_plat_base_glmakecurrent(ak_plat_base* pb)
{
}
