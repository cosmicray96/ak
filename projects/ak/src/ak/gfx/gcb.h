#ifndef ak_gfx_gcb_h
#define ak_gfx_gcb_h

#include "ak/coll/da.h"
#include "ak/core/math/mat3x3.h"
#include "ak/core/mem/allocator.h"
#include "ak/gfx/core.h"
#include "ak/gfx/mtrl.h"
#include "ak/gfx/mtrl/stg.h"

typedef struct
{
  ak_da cmds;
} ak_gcb;

ak_gcb
ak_gcb_make(ak_alct alct);
void
ak_gcb_destroy(ak_gcb* gcb);
void
ak_gcb_clear(ak_gcb* gcb);

void
ak_gcb_flush(ak_gcb* gcb,
             ak_gfx* gfx,
             ak_mtrlstg* ms);

void
ak_gcb_joinback(ak_gcb* dest, ak_gcb* src);

void
ak_gcb_push_mtrl(ak_gcb* gcb,
                 const ak_mtrl_basedata* bd,
                 const ak_mat3_f* vp,
                 ak_fx time);
void
ak_gcb_push_quad(ak_gcb* gcb,
                 const ak_mtrl_quaddata* qd,
                 const ak_mat3* gmat);

void
ak_gcb_push_scissor_reset(ak_gcb* gcb);
void
ak_gcb_push_scissor(ak_gcb* gcb,
                    int32_t x,
                    int32_t y,
                    uint32_t w,
                    uint32_t h);

void
ak_gcb_push_resize(ak_gcb* gcb,
                   uint32_t w,
                   uint32_t h);

#endif
