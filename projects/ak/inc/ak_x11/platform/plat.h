#ifndef ak_x11_platforn_plat_h
#define ak_x11_platforn_plat_h

#include "ak/app/eq.h"
#include "ak/core/mem/allocator.h"
#include <X11/Xlib.h>

typedef struct ak_plat ak_plat;
ak_plat*
ak_plat_startup(Display* d,
                Window wn,
                Atom wm_delete,
                uint32_t init_width,
                uint32_t init_height,
                ak_alct alct);
void
ak_plat_shutdown(ak_plat* p);

int32_t
ak_plat_width(ak_plat* p);
int32_t
ak_plat_height(ak_plat* p);

void
ak_plat_eventflush(ak_plat* p,
                   ak_app_eq* eq);

#endif
