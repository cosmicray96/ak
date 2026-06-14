#include "ak/res/astload.h"
#include "ak/gfx/core.h"
#include "ak/gfx/gresman.h"
#include "ak/res/reg.h"

ak_astload
ak_astload_make(ak_resman* rm,
                ak_gresman* grm,
                ak_thpool* tp,
                ak_alct alct)
{
  ak_astload al = { 0 };
  al.rm = rm;
  al.grm = grm;
  al.tp = tp;
  al.jids =
    ak_da_make(sizeof(ak_jobid), alct);
  return al;
}
void
ak_astload_destroy(ak_astload* al)
{
  ak_da_destroy(&al->jids);
  al->tp = 0;
  al->grm = 0;
  al->rm = 0;
}
void
ak_astload_update(ak_astload* al)
{
  ak_thpool_clear_done(al->tp, &al->jids);
}

void
ak_astload_res_image_from_stm(ak_astload* al,
                              ak_resid rid,
                              ak_stm stm)
{
  ak_resman_load(
    al->rm, rid, ak_restype_image, stm);
}

typedef struct
{
  ak_gresman* grm;
  ak_gresid gid;
  ak_resid rid;
  ak_textype textype;
  uint8_t phase;
} gres_tex_from_rid;
bool
gres_tex_from_rid_fn(void* ctx)
{
  return true;
}
void
ak_astload_gres_tex_from_rid(
  ak_astload* al,
  ak_gresid gid,
  ak_resid img_rid,
  ak_textype textype)
{
  gres_tex_from_rid s = { .grm = al->grm,
                          .gid = gid,
                          .rid = img_rid,
                          .textype =
                            textype };
}
