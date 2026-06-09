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
  ak_restype type;
  ak_stm stm;
} res_loading_item;

typedef struct
{
  ak_gresid gid;
  union
  {
    struct
    {
      bool tex_loading;
    } tex;
  };
} gres_loading_item;

typedef struct
{
  ak_restype type;
  ak_res_status status;
  const char* path;
} res_item;

typedef struct
{
  ak_grestype type;
  ak_gres_status status;
  union
  {
    struct
    {
      ak_resid img_rid;
      ak_textype type;
    } tex;
  };
} gres_item;

static void
tex_load_begin(ak_assetman* am,
               ak_gresid gid)
{
  gres_item* gi =
    ak_hmn_at(&am->gres_map, gid);

  ak_assetman_load_res(am, gi->tex.img_rid);
}

static bool
tex_load_update(ak_assetman* am,
                uint32_t idx)
{
  gres_loading_item* gli =
    ak_da_at(&am->gres_loadings, idx);
  gres_item* gi =
    ak_hmn_at(&am->gres_map, gli->gid);

  ak_res_status s = ak_resman_status(
    am->rm, gi->tex.img_rid);
  if (s != ak_res_loaded) {
    return false;
  }

  if (!gli->tex.tex_loading) {

    ak_img img = ak_resreg_get_image(
      ak_resman_resreg(am->rm),
      gi->tex.img_rid);
    ak_gresman_load_tex(
      am->grm, gli->gid, img, gi->tex.type);

    gli->tex.tex_loading = true;
    return false;
  }

  ak_gres_status gs =
    ak_gresman_status(am->grm, gli->gid);
  if (gs != ak_gres_loaded) {
    return false;
  }

  ak_assetman_unload_res(am,
                         gi->tex.img_rid);

  gi->status = ak_gres_loaded;
  return true;
}

static bool
gres_update(ak_assetman* am, uint32_t idx)
{
  gres_loading_item* gli =
    ak_da_at(&am->gres_loadings, idx);
  gres_item* gi =
    ak_hmn_at(&am->gres_map, gli->gid);

  switch (gi->type) {
    case ak_grestype_tex: {
      return tex_load_update(am, idx);
    }
    default: {
      ak_assert(false);
      return false;
    }
  }
}

//===== ak_assetman =====//
//--- private ---//
static ak_restype
res_type(ak_assetman* am, ak_resid rid)
{
  res_item* ri =
    ak_hmn_at(&am->res_map, rid);
  return ri->type;
}
static ak_grestype
gres_type(ak_assetman* am, ak_gresid rid)
{
  gres_item* gi =
    ak_hmn_at(&am->gres_map, rid);
  return gi->type;
}

//--- internal ---//
ak_assetman
ak_assetman_make(ak_resman* rm,
                 ak_gresman* grm,
                 ak_alct alct)
{
  ak_assetman am = { 0 };
  am.rm = rm;
  am.grm = grm;

  am.res_loadings = ak_da_make(
    sizeof(res_loading_item), alct);
  am.gres_loadings = ak_da_make(
    sizeof(gres_loading_item), alct);

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
      res_loading_item* rli =
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
      gres_loading_item* gli =
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

  res_loading_item rli = {
    .rid = rid,
    .type = ri->type,
    .stm = ak_stm_open_file(ri->path, "rb")
  };
  ak_da_pushback(&am->res_loadings, &rli);
  ak_resman_load(
    am->rm, rli.rid, rli.type, rli.stm);
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

  gres_loading_item gli = { .gid = gid };

  switch (gi->type) {
    case ak_grestype_tex: {
      gli.tex.tex_loading = false;
      ak_da_pushback(&am->gres_loadings,
                     &gli);
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
ak_assetman_reg_res_img(ak_assetman* am,
                        ak_resid rid,
                        const char* path)
{
  res_item ri = { .type = ak_restype_image,
                  .path = path };
  ak_hmn_insert(&am->res_map, rid, &ri);
}

void
ak_assetman_reg_res_world(ak_assetman* am,
                          ak_resid rid,
                          const char* path)
{
  res_item ri = { .type = ak_restype_world,
                  .path = path };
  ak_hmn_insert(&am->res_map, rid, &ri);
}

void
ak_assetman_reg_gres_tex(ak_assetman* am,
                         ak_gresid gid,
                         ak_resid img_rid,
                         ak_textype type)
{
  gres_item gi = {
    .type = ak_grestype_tex,
    .tex = { .img_rid = img_rid,
             .type = type },
    .status = ak_gres_not_loaded
  };
  ak_hmn_insert(&am->gres_map, gid, &gi);
}
