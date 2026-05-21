#ifndef ak_gfx_gcb_h
#define ak_gfx_gcb_h

#include "ak/coll/da.h"
#include "ak/core/math/fixed.h"
#include "ak/core/math/mat3x3.h"
#include "ak/core/mem/allocator.h"
#include "ak/gfx/core.h"
#include "ak/gfx/mtrl.h"
#include "ak/gfx/mtrl/stg.h"

typedef struct
{
  ak_mtrlstg* ms;
  ak_da mtrls;
  ak_da quads;
  ak_mat3 vp;
  ak_fx time;
} ak_gcb;

ak_gcb
ak_gcb_make(ak_mtrlstg* ms, ak_alct alct);
void
ak_gcb_destroy(ak_gcb* gcb);

void
ak_gcb_begin(ak_gcb* gcb,
             const ak_mat3* vp,
             ak_fx time);

void
ak_gcb_flush(ak_gcb* gcb, ak_gfx* gfx);

void
ak_gcb_push_mtrl(ak_gcb* gcb,
                 const ak_mtrl_basedata* bd);
void
ak_gcb_push_quad(ak_gcb* gcb,
                 const ak_mtrl_quaddata* qd,
                 const ak_mat3* gmat);

#endif
