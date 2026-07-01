#ifndef ak_system_render_h
#define ak_system_render_h

#include "ak/core/async/dispatcher.h"
#include "ak/core/mem/allocator.h"
#include "ak/gfx/gcb.h"
#include "ak/platform/plat_base.h"

typedef struct ak_renderer ak_renderer;
typedef void (*ak_renderer_fn)(void* ctx);

typedef enum
{
  ak_renderer_initing,
  ak_renderer_rendering,
  ak_renderer_idle,
} ak_renderer_status;

ak_renderer*
ak_renderer_startup(ak_plat_base* pb,
                    ak_resreg* rr,
                    ak_alct alct);

void
ak_renderer_shutdown(ak_renderer* r);

void
ak_renderer_render(ak_renderer* r,
                   ak_gcb* gcb);

ak_renderer_status
ak_renderer_status_get(ak_renderer* r);

ak_dispatcher*
ak_renderer_dispatcher_pre(ak_renderer* r);
ak_dispatcher*
ak_renderer_dispatcher(ak_renderer* r);
ak_gfx*
ak_renderer_gfx(ak_renderer* r);

ak_renderer*
ak_renderer_get();

#endif
