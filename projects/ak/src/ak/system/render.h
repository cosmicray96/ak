#ifndef ak_system_render_h
#define ak_system_render_h

#include "ak/core/mem/allocator.h"
#include "ak/game/world/view.h"
#include "ak/gfx/core.h"
#include "ak/gfx/gcb.h"
#include "ak/gfx/gresman.h"
#include "ak/platform/plat_base.h"

typedef struct ak_renderer ak_renderer;

typedef enum
{
  ak_renderer_initing,
  ak_renderer_rendering,
  ak_renderer_idle,
} ak_renderer_status;

ak_renderer*
ak_renderer_startup(ak_plat_base* pb,
                    ak_alct alct);

void
ak_renderer_shutdown(ak_renderer* r);

ak_gresman*
ak_renderer_gresman_get(ak_renderer* r);
ak_gresreg*
ak_renderer_gresreg_get(ak_renderer* r);

void
ak_renderer_render(ak_renderer* r,
                   ak_gcb* gcb);

ak_renderer_status
ak_renderer_status_get(ak_renderer* r);

#endif
