#ifndef ak_renderer_renderer_h
#define ak_renderer_renderer_h

#include "ak/core/mem/allocator.h"
#include "ak/renderer/core.h"
#include <stdint.h>

ak_renderer*
ak_renderer_startup(ak_alct alct);
void
ak_renderer_shutdown(ak_renderer* r);

void
ak_renderer_render(ak_renderer* r);

void
ak_renderer_resize(ak_renderer* r,
                   int32_t w,
                   int32_t h);

#endif
