#ifndef ak_gfx_mtrl_vcol
#define ak_gfx_mtrl_vcol

#include "ak/core/math/vec2.h"
#include "ak/core/math/vec4.h"
#include "ak/core/mem/allocator.h"
#include "ak/gfx/core.h"
typedef struct ak_mtrl_vcol ak_mtrl_vcol;

ak_mtrl_vcol*
ak_mtrl_vcol_make(ak_alct alct);
void
ak_mtrl_vcol_destroy(ak_mtrl_vcol* m);

void
ak_mtrl_gfx_begin(ak_mtrl_vcol* m,
                  ak_gfx* g);

void
ak_mtrl_vcol_pushvert(ak_mtrl_vcol* m,
                      ak_gfx* g,
                      ak_vec2 vert_pos,
                      ak_vec4 vert_col);

#endif
