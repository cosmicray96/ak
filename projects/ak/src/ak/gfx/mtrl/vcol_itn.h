#ifndef ak_gfx_mtrl_vcol_itn_h
#define ak_gfx_mtrl_vcol_itn_h

#include "ak/core/math/vec2.h"
#include "ak/core/math/vec4.h"
#include "ak/core/mem/allocator.h"
#include "ak/gfx/core.h"
#include "ak/gfx/mtrl/mtrl.h"

ak_ex ak_mtrl_vcol*
ak_mtrl_vcol_make(ak_gfx* g, ak_alct alct);

ak_ex void
ak_mtrl_vcol_destroy(ak_mtrl_vcol* m);

ak_ex void
ak_mtrl_vcol_call_begin(ak_mtrl_vcol* m);
ak_ex void
ak_mtrl_vcol_call_end(ak_mtrl_vcol* m);

ak_ex void
ak_mtrl_vcol_pushvert(ak_mtrl_vcol* m,
                      ak_vec2 vert_pos,
                      ak_vec4 vert_col);

#endif
