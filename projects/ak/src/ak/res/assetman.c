/*
#include "ak/res/assetman.h"
#include "ak/debug.h"
#include "ak/gfx/gresman.h"
#include "ak/gfx/gresreg.h"
#include "ak/gfx/tex.h"
#include "ak/res/reg.h"
#include "ak/res/resman_itn.h"
#include "ak/system/stream.h"

//--- private ---//
typedef struct
{
  ak_resid rid;
  ak_stm stm;
} res_cache;

typedef struct
{
  ak_gresid gid;
  bool gid_loading;
} cache_item;

typedef struct
{
  ak_assetman_args args;
  ak_gres_status status;
} gres_item;

typedef struct
{
  ak_restype type;
  const char* path;
} res_item;

static void
tex_load_begin(ak_assetman* am,
               ak_gresid gid)
{
  gres_item* gi =
    ak_hmn_at(&am->gres_map, gid);

  ak_assetman_load_res(am, gi->args.rid);
}

static bool
tex_load_update(ak_assetman* am,
                uint32_t idx)
{
  cache_item* gli =
    ak_da_at(&am->gres_loadings, idx);
  gres_item* gi =
    ak_hmn_at(&am->gres_map, gli->gid);

  if (!gli->gid_loading) {

    ak_img img = ak_resreg_get_image(
      ak_resman_resreg(am->rm),
      gi->args.rid);
    ak_gresman_args args = {
      .type = ak_grestype_tex,
      .tex = { .img = img,
               .type = gi->args.tex.textype }
    };

    ak_gresman_load(am->grm, gli->gid, args);
    gli->gid_loading = true;
  }
  return false;
}

static bool
gres_update(ak_assetman* am, uint32_t idx)
{
  cache_item* gli =
    ak_da_at(&am->gres_loadings, idx);
  gres_item* gi =
    ak_hmn_at(&am->gres_map, gli->gid);

  ak_res_status s =
    ak_resman_status(am->rm, gi->args.rid);
  if (s != ak_res_loaded) {
    return false;
  }

  switch (gi->args.type) {
    case ak_grestype_tex: {
      return tex_load_update(am, idx);
    }
    default: {
      ak_assert(false);
      return false;
    }
  }

  ak_gres_status gs =
    ak_gresman_status(am->grm, gli->gid);
  if (gs != ak_gres_loaded) {
    return false;
  }

  ak_assetman_unload_res(am, gi->args.rid);

  gi->status = ak_gres_loaded;
  return true;
}

//===== ak_assetman =====//
//--- internal ---//
ak_assetman
ak_assetman_make(ak_resman* rm,
                 ak_gresman* grm,
                 ak_alct alct)
{
  ak_assetman am = { 0 };
  am.rm = rm;
  am.grm = grm;

  am.res_loadings =
    ak_da_make(sizeof(res_cache), alct);
  am.gres_loadings =
    ak_da_make(sizeof(cache_item), alct);

  am.res_map =
    ak_hmn_make(sizeof(res_item), alct);
  am.gres_map =
    ak_hmn_make(sizeof(gres_item), alct);

  am.gres_unloadings =
    ak_da_make(sizeof(ak_gresid), alct);

  return am;
}

void
ak_assetman_destroy(ak_assetman* am)
{
  ak_log("fix assetman");

  ak_da_destroy(&am->gres_unloadings);

  ak_da_destroy(&am->res_loadings);
  ak_da_destroy(&am->gres_loadings);
  ak_hmn_destroy(&am->res_map);
  ak_hmn_destroy(&am->gres_map);
  am->rm = 0;
  am->grm = 0;
}

void
ak_assetman_update(ak_assetman* am)
{
  // res loadings
  {
    uint32_t count =
      ak_da_count(&am->res_loadings);
    uint32_t i = 0;
    while (i < count) {
      res_cache* rli =
        ak_da_at(&am->res_loadings, i);

      ak_res_status s =
        ak_resman_status(am->rm, rli->rid);
      if (s == ak_res_loaded ||
          s == ak_res_not_exist) {
        ak_stm_close(rli->stm);
        ak_da_remove_swaplast(
          &am->res_loadings, i);
        count--;
      } else {
        i++;
      }
    }
  }

  // gres loadings
  {
    uint32_t count =
      ak_da_count(&am->gres_loadings);
    uint32_t i = 0;
    while (i < count) {
      cache_item* gli =
        ak_da_at(&am->gres_loadings, i);

      bool done = gres_update(am, i);
      if (done) {
        ak_da_remove_swaplast(
          &am->gres_loadings, i);
        count--;
      } else {
        i++;
      }
    }
  }

  // gres unloadings
  {
    uint32_t count =
      ak_da_count(&am->gres_unloadings);
    uint32_t i = 0;
    while (i < count) {
      ak_gresid gid = *(ak_gresid*)ak_da_at(
        &am->gres_unloadings, i);
      gres_item* gi =
        ak_hmn_at(&am->gres_map, gid);
      if (gi->status == ak_gres_loaded) {
        ak_gresman_unload(am->grm, gid);

        ak_da_remove_swaplast(
          &am->gres_unloadings, i);
        gi->status = ak_gres_not_loaded;
        count--;
      } else {
        i++;
      }
    }
  }
}

void
ak_assetman_load_res(ak_assetman* am,
                     ak_resid rid)
{
  ak_assert(ak_hmn_exist(&am->res_map, rid));

  res_item* ri =
    ak_hmn_at(&am->res_map, rid);

  res_cache rli = { .rid = rid,
                    .stm = ak_stm_open_file(
                      ri->path, "rb") };
  ak_da_pushback(&am->res_loadings, &rli);
  ak_resman_load(
    am->rm, rid, ri->type, rli.stm);
}

void
ak_assetman_unload_res(ak_assetman* am,
                       ak_resid rid)
{
  ak_assert(ak_hmn_exist(&am->res_map, rid));

  ak_resman_unload(am->rm, rid);
}

void
ak_assetman_load_gres(ak_assetman* am,
                      ak_gresid gid)
{
  ak_assert(
    ak_hmn_exist(&am->gres_map, gid));
  gres_item* gi =
    ak_hmn_at(&am->gres_map, gid);
  if (gi->status != ak_gres_not_loaded) {
    return;
  }
  gi->status = ak_gres_loading;

  cache_item gli = { .gid = gid,
                     .gid_loading = false };
  ak_da_pushback(&am->gres_loadings, &gli);

  switch (gi->args.type) {
    case ak_grestype_tex: {
      tex_load_begin(am, gid);
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

  gres_item* gi =
    ak_hmn_at(&am->gres_map, gid);
  if (gi->status == ak_gres_not_loaded) {
    return;
  }

  ak_da_pushback(&am->gres_unloadings, &gid);
}

void
ak_assetman_reg_res(ak_assetman* am,
                    ak_resid rid,
                    ak_restype type,
                    const char* path)
{
  res_item ri = { .type = type,
                  .path = path };
  ak_hmn_insert(&am->res_map, rid, &ri);
}

void
ak_assetman_reg_gres(
  ak_assetman* am,
  ak_gresid gid,
  const ak_assetman_args* args)
{
  gres_item gi = { .args = *args,
                   .status =
                     ak_gres_not_loaded };
  ak_hmn_insert(&am->gres_map, gid, &gi);
}
*/
