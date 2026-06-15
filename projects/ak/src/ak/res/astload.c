#include "ak/res/astload.h"
#include "ak/core/img.h"
#include "ak/debug.h"
#include "ak/gfx/core.h"
#include "ak/gfx/gresman.h"
#include "ak/res/reg.h"
#include "ak/res/resman_itn.h"

typedef struct
{
  ak_resman* rm;
  ak_gresman* grm;
  ak_gresid gid;
  ak_textype textype;
  ak_resid rid;
  ak_stm stm;
  uint8_t phase;
} gres_tex;
bool
gres_tex_fn(void* ctx)
{
  gres_tex* gt = ctx;
  switch (gt->phase) {
    case 0: {
      ak_resman_load(gt->rm,
                     gt->rid,
                     ak_restype_image,
                     gt->stm,
                     true);
      gt->phase = 1;
      return false;
    }
    case 1: {
      if (ak_resman_status(gt->rm,
                           gt->rid) !=
          ak_res_loaded) {
        return false;
      }
      ak_img img = ak_resreg_get_image(
        ak_resman_resreg(gt->rm), gt->rid);

      ak_gresman_load_tex(
        gt->grm, gt->gid, &img, gt->textype);
      gt->phase = 2;
      return false;
    }
    case 2: {
      if (ak_gresman_status(gt->grm,
                            gt->gid) !=
          ak_gres_loaded) {
        return false;
      }

      ak_resman_unload(gt->rm, gt->rid);
      return true;
    }
  }
  ak_assert(false);
  return false;
}

void
ak_astload_gres_load_tex(ak_thpool* th,
                         ak_resman* rm,
                         ak_gresman* grm,
                         ak_gresid gid,
                         ak_textype textype,
                         ak_resid rid,
                         ak_stm stm)
{
  gres_tex gt = { .rm = rm,
                  .grm = grm,
                  .gid = gid,
                  .textype = textype,
                  .rid = rid,
                  .stm = stm,
                  .phase = 0 };
  ak_thpool_submit(th,
                   &gres_tex_fn,
                   sizeof(gres_tex),
                   &gt,
                   true);
}

typedef struct
{
  ak_gresman* grm;
  ak_gresid gid;
} gres_tex_unload;
bool
gres_tex_unload_fn(void* ctx)
{
  gres_tex_unload* gt = ctx;
  ak_gres_status status =
    ak_gresman_status(gt->grm, gt->gid);
  if (status == ak_gres_not_exist) {
    return false;
  }
  ak_gresman_unload(gt->grm, gt->gid);
  return true;
}
void
ak_astload_gres_unload_tex(ak_thpool* th,
                           ak_gresman* grm,
                           ak_gresid gid)
{

  gres_tex_unload gt = {
    .grm = grm,
    .gid = gid,
  };
  ak_thpool_submit(th,
                   &gres_tex_unload_fn,
                   sizeof(gres_tex_unload),
                   &gt,
                   true);
}
