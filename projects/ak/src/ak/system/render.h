#ifndef ak_system_render_h
#define ak_system_render_h

#include "ak/core/mem/allocator.h"
#include "ak/game/world/view.h"
#include "ak/gfx/core.h"
#include "ak/gfx/gresman.h"
#include "ak/platform/plat_base.h"

typedef struct ak_renderer ak_renderer;

ak_renderer*
ak_renderer_startup(ak_plat_base* pb,
                    ak_wv* wv,
                    ak_resman* rm,
                    ak_alct alct);

void
ak_renderer_shutdown(ak_renderer* r);

ak_gfx*
ak_renderer_gfx(ak_renderer* r);
ak_gresman*
ak_renderer_gresman(ak_renderer* r);

void
ak_renderer_render(ak_renderer* r);

void
ak_renderer_stallwait(ak_renderer* r);

#endif
