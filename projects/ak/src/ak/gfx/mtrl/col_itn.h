#ifndef ak_gfx_mtrl_col_itn_h
#define ak_gfx_mtrl_col_itn_h

#include "ak/core/math/vec2.h"
#include "ak/core/mem/allocator.h"
#include "ak/gfx/core.h"
#include "ak/gfx/mtrl/mtrl.h"

ak_mtrl_col*
ak_mtrl_col_make(ak_gfx* g, ak_alct alct);
void
ak_mtrl_col_destroy(ak_mtrl_col* m);

void
ak_mtrl_col_call_begin(ak_mtrl_col* m);
void
ak_mtrl_col_call_end(ak_mtrl_col* m);

void
ak_mtrl_col_pushvert(ak_mtrl_col* m,
                     ak_vec2 vert_pos);

#endif
