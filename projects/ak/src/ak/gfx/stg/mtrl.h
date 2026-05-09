#ifndef ak_gfx_stg_mtrl_h
#define ak_gfx_stg_mtrl_h

#include "ak/core/mem/allocator.h"
#include "ak/gfx/core.h"
#include "ak/gfx/mtrl.h"
typedef struct ak_mtrlstg ak_mtrlstg;

ak_mtrlstg*
ak_mtrlstg_make(ak_gfx* g, ak_alct alct);
void
ak_mtrlstg_destroy(ak_mtrlstg* ms);

ak_mtrl
ak_mtrlstg_at(ak_mtrlstg* ms,
              ak_mtrl_enum me);

#endif
