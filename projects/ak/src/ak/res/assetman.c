#include "ak/res/assetman.h"
#include "ak/debug.h"
#include "ak/gfx/gresman.h"
#include "ak/gfx/gresreg.h"
#include "ak/res/astload.h"
#include "ak/res/reg.h"
#include "ak/system/stream.h"

//--- private ---//
typedef struct
{
  ak_assetman_gargs gargs;
} gres_item;

typedef struct
{
  ak_assetman_rargs rargs;
} res_item;

//===== ak_assetman =====//
//--- internal ---//
ak_assetman
ak_assetman_make(ak_thpool* tp,
                 ak_resman* rm,
                 ak_gresman* grm,
                 ak_alct alct)
{
  ak_assetman am = { 0 };
  am.tp = tp;
  am.rm = rm;
  am.grm = grm;

  am.res_map =
    ak_hmn_make(sizeof(res_item), alct);
  am.gres_map =
    ak_hmn_make(sizeof(gres_item), alct);

  return am;
}

void
ak_assetman_destroy(ak_assetman* am)
{
  ak_hmn_destroy(&am->res_map);
  ak_hmn_destroy(&am->gres_map);
  am->rm = 0;
  am->grm = 0;
}

void
ak_assetman_load_res(ak_assetman* am,
                     ak_resid rid)
{
  ak_assert(ak_hmn_exist(&am->res_map, rid));

  res_item* ri =
    ak_hmn_at(&am->res_map, rid);

  ak_stm stm = { 0 };
  ak_stmerr err = ak_stm_open_file(
    ri->rargs.path, "rb", &stm);
  ak_assert(err == ak_stmerr_ok);

  switch (ri->rargs.type) {
    case ak_restype_image:
    case ak_restype_world: {
      ak_resman_load(
        am->rm,
        rid,
        &(ak_resman_args){
          .type = ri->rargs.type,
          .stm = stm,
          .stm_close = true });
      break;
    }

    case ak_restype_texatlas: {
      ak_assetman_load_gres(
        am, ri->rargs.texatlas.tex_gid);
      ak_astload_res_load_texatlas(
        am->tp,
        am->rm,
        am->grm,
        rid,
        stm,
        ri->rargs.texatlas.tex_gid);
      break;
    }
    case ak_restype_aniclip: {
      ak_assetman_load_res(
        am, ri->rargs.aniclip.atlas_rid);
      ak_astload_res_load_aniclip(
        am->tp,
        am->rm,
        rid,
        stm,
        ri->rargs.aniclip.atlas_rid);
      break;
    }
    default: {
      ak_assert(false);
    }
  }
}

void
ak_assetman_unload_res(ak_assetman* am,
                       ak_resid rid)
{
  ak_assert(ak_hmn_exist(&am->res_map, rid));
  res_item* ri =
    ak_hmn_at(&am->res_map, rid);
  switch (ri->rargs.type) {
    case ak_restype_image:
    case ak_restype_world:
    case ak_restype_shaderstr: {
      break;
    }
    case ak_restype_texatlas: {
      ak_gresman_unload(
        am->grm, ri->rargs.texatlas.tex_gid);
      break;
    }
    case ak_restype_aniclip: {
      ak_gresman_unload(
        am->grm,
        ri->rargs.aniclip.atlas_rid);
    }
    default: {
      ak_assert(false);
    }
  }
  ak_astload_res_unload(am->tp, am->rm, rid);
}

void
ak_assetman_load_gres(ak_assetman* am,
                      ak_gresid gid)
{
  ak_assert(
    ak_hmn_exist(&am->gres_map, gid));
  gres_item* gi =
    ak_hmn_at(&am->gres_map, gid);

  res_item* ri =
    ak_hmn_at(&am->res_map, gi->gargs.rid);
  ak_stm stm = { 0 };
  ak_stmerr err = ak_stm_open_file(
    ri->rargs.path, "rb", &stm);
  ak_assert(err == ak_stmerr_ok);

  switch (gi->gargs.type) {
    case ak_grestype_tex: {
      ak_resman_load(
        am->rm,
        gi->gargs.rid,
        &(ak_resman_args){
          .type = ak_restype_image,
          .stm = stm,
          .stm_close = true });
      ak_astload_gres_load_tex(
        am->tp,
        am->rm,
        am->grm,
        gid,
        gi->gargs.tex.textype,
        gi->gargs.rid);
      break;
    }
    case ak_grestype_shader: {
      ak_resman_load(
        am->rm,
        gi->gargs.rid,
        &(ak_resman_args){
          .type = ak_restype_shaderstr,
          .stm = stm,
          .stm_close = true });
      ak_astload_gres_load_shader(
        am->tp,
        am->rm,
        am->grm,
        gid,
        gi->gargs.rid);
      break;
    }
    default: {
      ak_assert(false);
    }
  }
}

void
ak_assetman_unload_gres(ak_assetman* am,
                        ak_gresid gid)
{
  ak_assert(
    ak_hmn_exist(&am->gres_map, gid));

  ak_astload_gres_unload(
    am->tp, am->grm, gid);
}

void
ak_assetman_reg_res(
  ak_assetman* am,
  ak_resid rid,
  const ak_assetman_rargs* rargs)
{
  res_item ri = { .rargs = *rargs };
  ak_hmn_insert(&am->res_map, rid, &ri);
}

void
ak_assetman_reg_gres(
  ak_assetman* am,
  ak_gresid gid,
  const ak_assetman_gargs* gargs)
{
  gres_item gi = { .gargs = *gargs };
  ak_hmn_insert(&am->gres_map, gid, &gi);
}
