#include "ak/res/astload.h"
#include "ak/debug.h"
#include "ak/gfx/gresman.h"
#include "ak/res/reg.h"
#include "ak/res/reses/img.h"
#include "ak/res/resman_itn.h"

typedef struct
{
  ak_resman* rm;
  ak_resid rid;
} res_unload;
bool
res_unload_fn(void* ctx)
{
  res_unload* ru = ctx;
  ak_res_status status =
    ak_resman_status(ru->rm, ru->rid);
  if (status == ak_res_not_exist) {
    return false;
  }
  ak_resman_unload(ru->rm, ru->rid);
  return true;
}
void
ak_astload_res_unload(ak_thpool* tp,
                      ak_resman* rm,
                      ak_resid rid)
{
  ak_thpool_submit(
    tp,
    &res_unload_fn,
    &(res_unload){ .rm = rm, .rid = rid },
    sizeof(res_unload),
    true);
}

typedef struct
{
  ak_gresman* grm;
  ak_gresid gid;
} gres_unload;
bool
gres_unload_fn(void* ctx)
{
  gres_unload* gu = ctx;
  ak_gres_status status =
    ak_gresman_status(gu->grm, gu->gid);
  if (status == ak_gres_not_exist) {
    return false;
  }
  ak_gresman_unload(gu->grm, gu->gid);
  return true;
}
void
ak_astload_gres_unload(ak_thpool* tp,
                       ak_gresman* grm,
                       ak_gresid gid)
{
  gres_unload gu = {
    .grm = grm,
    .gid = gid,
  };
  ak_thpool_submit(tp,
                   &gres_unload_fn,
                   &gu,
                   sizeof(gres_unload),
                   true);
}

typedef struct
{
  ak_resman* rm;
  ak_gresman* grm;
  ak_resid ta_rid;
  ak_stm stm;
  ak_gresid tex_gid;
} res_texatlas;
bool
res_texatlas_fn(void* ctx)
{
  ak_log("fix, needs to unload.");
  res_texatlas* rta = ctx;
  ak_gres_status tex_status =
    ak_gresman_status(rta->grm,
                      rta->tex_gid);
  if (tex_status == ak_gres_loaded) {
    ak_resman_load(
      rta->rm,
      rta->ta_rid,
      &(ak_resman_args){
        .type = ak_restype_texatlas,
        .stm = rta->stm,
        .stm_close = true,
        .gid = rta->tex_gid });
    return true;
  }
  return false;
}
void
ak_astload_res_load_texatlas(
  ak_thpool* th,
  ak_resman* rm,
  ak_gresman* grm,
  ak_resid ta_rid,
  ak_stm stm,
  ak_gresid tex_gid)
{
  ak_thpool_submit(
    th,
    &res_texatlas_fn,
    &(res_texatlas){ .rm = rm,
                     .grm = grm,
                     .ta_rid = ta_rid,
                     .tex_gid = tex_gid,
                     .stm = stm },
    sizeof(res_texatlas),
    true);
}

typedef struct
{
  ak_resman* rm;
  ak_resid ac_rid;
  ak_stm stm;
  ak_resid ta_rid;
  uint8_t phase;
} res_ac;
bool
res_ac_fn(void* ctx)
{
  ak_log("fix, needs to unload.");
  res_ac* rac = ctx;
  ak_res_status ta_status =
    ak_resman_status(rac->rm, rac->ta_rid);
  if (ta_status == ak_res_loaded) {
    ak_resman_load(
      rac->rm,
      rac->ta_rid,
      &(ak_resman_args){
        .type = ak_restype_texatlas,
        .stm = rac->stm,
        .stm_close = true,
        .rid = rac->ta_rid });
    return true;
  }
  return false;
}
void
ak_astload_res_load_aniclip(ak_thpool* th,
                            ak_resman* rm,
                            ak_resid ac_rid,
                            ak_stm stm,
                            ak_resid ta_rid)
{
  ak_thpool_submit(
    th,
    &res_ac_fn,
    &(res_ac){ .rm = rm,
               .ac_rid = ac_rid,
               .stm = stm,
               .ta_rid = ta_rid,
               .phase = 0 },
    sizeof(res_ac),
    true);
}

typedef struct
{
  ak_resman* rm;
  ak_gresman* grm;
  ak_gresid gid;
  ak_textype textype;
  ak_resid rid;
  uint8_t phase;
} gres_tex;
bool
gres_tex_fn(void* ctx)
{
  gres_tex* gt = ctx;
  switch (gt->phase) {
    case 0: {
      if (ak_resman_status(gt->rm,
                           gt->rid) !=
          ak_res_loaded) {
        return false;
      }
      ak_img img = ak_resreg_get_image(
        ak_resman_resreg(gt->rm), gt->rid);

      ak_gresman_load_tex(
        gt->grm, gt->gid, &img, gt->textype);
      gt->phase = 1;
      return false;
    }
    case 1: {
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
                         ak_gresid tex_gid,
                         ak_textype textype,
                         ak_resid img_rid)
{
  gres_tex gt = { .rm = rm,
                  .grm = grm,
                  .gid = tex_gid,
                  .textype = textype,
                  .rid = img_rid,
                  .phase = 0 };
  ak_thpool_submit(th,
                   &gres_tex_fn,
                   &gt,
                   sizeof(gres_tex),
                   true);
}

typedef struct
{
  ak_resman* rm;
  ak_gresman* grm;
  ak_gresid gid;
  ak_resid rid;
  ak_stm stm;
  uint8_t phase;
} gres_shader;
bool
gres_shader_fn(void* ctx)
{
  gres_shader* gs = ctx;
  switch (gs->phase) {
    case 0: {
      if (ak_resman_status(gs->rm,
                           gs->rid) !=
          ak_res_loaded) {
        return false;
      }
      ak_shaderstr ss =
        ak_resreg_get_shaderstr(
          ak_resman_resreg(gs->rm), gs->rid);

      ak_gresman_load_shader(
        gs->grm, gs->gid, &ss);
      gs->phase = 1;
      return false;
    }
    case 1: {
      if (ak_gresman_status(gs->grm,
                            gs->gid) !=
          ak_gres_loaded) {
        return false;
      }

      ak_resman_unload(gs->rm, gs->rid);
      return true;
    }
  }
  ak_assert(false);
  return false;
}
void
ak_astload_gres_load_shader(ak_thpool* th,
                            ak_resman* rm,
                            ak_gresman* grm,
                            ak_gresid s_gid,
                            ak_resid ss_rid)
{
  gres_shader gs = { .rm = rm,
                     .grm = grm,
                     .gid = s_gid,
                     .rid = ss_rid,
                     .phase = 0 };
  ak_thpool_submit(th,
                   &gres_shader_fn,
                   &gs,
                   sizeof(gres_shader),
                   true);
}
