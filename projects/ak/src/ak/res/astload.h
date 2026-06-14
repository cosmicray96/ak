#ifndef ak_res_astload_h
#define ak_res_astload_h

#include "ak/coll/da.h"
#include "ak/core/async/thpool.h"
#include "ak/core/mem/allocator.h"
#include "ak/gfx/core.h"
#include "ak/gfx/gresman.h"
#include "ak/gfx/gresreg.h"
#include "ak/res/reg.h"
#include "ak/res/resman.h"
#include "ak/system/stream.h"

typedef struct
{
  ak_resman* rm;
  ak_gresman* grm;
  ak_thpool* tp;
  ak_da jids;
} ak_astload;

ak_astload
ak_astload_make(ak_resman* rm,
                ak_gresman* grm,
                ak_thpool* tp,
                ak_alct alct);
void
ak_astload_destroy(ak_astload* al);

void
ak_astload_update(ak_astload* al);

void
ak_astload_res_image_from_stm(ak_astload* al,
                              ak_resid rid,
                              ak_stm stm);
void
ak_astload_gres_tex_from_rid(
  ak_astload* al,
  ak_gresid gid,
  ak_resid img_rid,
  ak_textype type);

#endif
