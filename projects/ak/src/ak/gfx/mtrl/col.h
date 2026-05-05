#ifndef ak_gfx_mtrl_col
#define ak_gfx_mtrl_col

#include "ak/core/math/vec2.h"
#include "ak/core/math/vec4.h"
#include "ak/core/mem/allocator.h"
#include "ak/gfx/core.h"
typedef struct ak_mtrl_col ak_mtrl_col;

ak_mtrl_col*
ak_mtrl_col_make(ak_gfx* g, ak_alct alct);
void
ak_mtrl_col_destroy(ak_mtrl_col* m);

void
ak_mtrl_col_col_set(ak_mtrl_col* m,
                    ak_vec4 col);

void
ak_mtrl_col_call_begin(ak_mtrl_col* m);
void
ak_mtrl_col_call_end(ak_mtrl_col* m);

void
ak_mtrl_col_pushvert(ak_mtrl_col* m,
                     ak_vec2 vert_pos);

#endif
