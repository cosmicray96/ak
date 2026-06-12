#include "ak/gfx/gresman.h"
#include "ak/coll/dq.h"
#include "ak/coll/hmn.h"
#include "ak/core/async/mutex.h"
#include "ak/core/img.h"
#include "ak/core/mem/allocator.h"
#include "ak/core/shaderstr.h"
#include "ak/debug.h"
#include "ak/gfx/core.h"
#include "ak/gfx/gresreg.h"
#include "ak/gfx/shader.h"
#include "ak/gfx/tex.h"

//===== gres_item =====//
//--- private ---//

typedef struct
{
  ak_gresid id;
  ak_grestype type;
  bool success;
  union
  {
    ak_tex* tex;
    ak_shader* shader;
  };
} loaded_item;

typedef struct
{
  ak_gresman* grm;
  ak_gresid id;
  ak_grestype type;
  union
  {
    struct
    {
      ak_img img;
      ak_textype type;
    } tex;
    ak_shaderstr ss;
  };
} gres_loading_item;

typedef struct
{
  ak_grestype type;
  ak_gres_status s;
  uint32_t load_count;
} gres_item;

//===== ak_gresman =====//
//--- private ---//
struct ak_gresman
{
  ak_alct alct;
  ak_gresreg* grr;
  ak_gfx* gfx;

  ak_mutex m;

  ak_hmn map;
  ak_dq loadings;
  ak_dq loaded;
  ak_dq unloadings;
};

static void
gres_load_unsafe(gres_loading_item li)
{
  loaded_item loaded = { .id = li.id,
                         .type = li.type };
  switch (li.type) {
    case ak_grestype_tex: {
      loaded.tex =
        ak_tex_make_from_img(li.grm->gfx,
                             &li.tex.img,
                             li.tex.type,
                             li.grm->alct);
      loaded.success = true;
      break;
    }
    case ak_grestype_shader: {
      loaded.shader =
        ak_shader_from_shaderstr(
          li.grm->gfx, &li.ss, li.grm->alct);
      loaded.success = true;
      break;
    }
    default: {
      ak_assert(false);
    }
  }
  ak_dq_push(&li.grm->loaded, &loaded);
}

static void
gres_unload_unsafe(ak_gresman* grm,
                   ak_gresid id)
{
  ak_gresreg_unreg(grm->grr, id);
  ak_hmn_remove(&grm->map, id);
}

//--- internal ---//
ak_gresman*
ak_gresman_startup(ak_gresreg* grr,
                   ak_gfx* gfx,
                   ak_alct alct)
{
  ak_gresman* grm =
    ak_alct_alloc(alct, sizeof(ak_gresman));
  grm->alct = alct;
  grm->gfx = gfx;
  grm->grr = grr;
  grm->map =
    ak_hmn_make(sizeof(gres_item), alct);
  grm->loadings = ak_dq_make(
    sizeof(gres_loading_item), alct);
  grm->loaded =
    ak_dq_make(sizeof(loaded_item), alct);
  grm->unloadings =
    ak_dq_make(sizeof(ak_gresid), alct);

  grm->m = ak_mutex_make();
  return grm;
}

void
ak_gresman_shutdown(ak_gresman* grm)
{

  ak_mutex_destroy(&grm->m);

  ak_dq_destroy(&grm->loadings);
  ak_dq_destroy(&grm->unloadings);
  ak_hmn_destroy(&grm->map);
  ak_alct_free(grm->alct, grm);
}

void
ak_gresman_update(ak_gresman* grm)
{
  ak_mutex_lock(&grm->m);

  {
    ak_gresid id = 0;
    if (ak_dq_pop(&grm->unloadings, &id)) {
      gres_item* gi =
        ak_hmn_at(&grm->map, id);
      if (gi->load_count == 0 &&
          gi->s == ak_gres_loaded) {
        gres_unload_unsafe(grm, id);
      } else {
        ak_dq_push(&grm->unloadings, &id);
      }
    }
  }

  {
    gres_loading_item li = { 0 };
    if (ak_dq_pop(&grm->loadings, &li)) {
      gres_load_unsafe(li);
    }
  }

  {
    loaded_item ldi = { 0 };
    while (ak_dq_pop(&grm->loaded, &ldi)) {
      ak_gresreg_reg(
        grm->grr, ldi.id, ldi.type, ldi.tex);
      gres_item* gi =
        ak_hmn_at(&grm->map, ldi.id);
      gi->s = ak_gres_loaded;
    }
  }
  ak_mutex_unlock(&grm->m);
}

//--- export ---//
ak_gres_status
ak_gresman_status(ak_gresman* grm,
                  ak_gresid id)
{

  ak_mutex_lock(&grm->m);

  if (!ak_hmn_exist(&grm->map, id)) {
    ak_mutex_unlock(&grm->m);
    return ak_gres_not_exist;
  }
  gres_item* gi = ak_hmn_at(&grm->map, id);
  ak_gres_status s = gi->s;
  ak_mutex_unlock(&grm->m);
  return s;
}

void
ak_gresman_unload(ak_gresman* grm,
                  ak_gresid id)
{
  ak_mutex_lock(&grm->m);

  gres_item* gi = ak_hmn_at(&grm->map, id);

  ak_assert(gi->load_count > 0);

  gi->load_count--;
  if (gi->load_count == 0) {
    ak_dq_push(&grm->unloadings, &id);
  }

  ak_mutex_unlock(&grm->m);
}

void
ak_gresman_load_tex(ak_gresman* grm,
                    ak_gresid id,
                    ak_img img,
                    ak_textype type)
{
  ak_mutex_lock(&grm->m);

  if (ak_hmn_exist(&grm->map, id)) {
    gres_item* gi = ak_hmn_at(&grm->map, id);
    gi->load_count++;
    ak_mutex_unlock(&grm->m);
    return;
  }

  gres_item gi = { .type = ak_grestype_tex,
                   .s = ak_gres_loading,
                   .load_count = 1 };
  ak_hmn_insert(&grm->map, id, &gi);

  gres_loading_item li = {
    .grm = grm,
    .id = id,
    .type = ak_grestype_tex,
    .tex = { .img = img, .type = type }
  };
  ak_dq_push(&grm->loadings, &li);

  ak_mutex_unlock(&grm->m);
}

void
ak_gresman_load_shader(ak_gresman* grm,
                       ak_gresid id,
                       ak_shaderstr ss)
{
  ak_mutex_lock(&grm->m);

  if (ak_hmn_exist(&grm->map, id)) {
    gres_item* gi = ak_hmn_at(&grm->map, id);
    gi->load_count++;
    ak_mutex_unlock(&grm->m);
    return;
  }

  gres_item gi = { .type = ak_grestype_tex,
                   .s = ak_gres_loading,
                   .load_count = 1 };
  ak_hmn_insert(&grm->map, id, &gi);

  gres_loading_item li = {
    .grm = grm,
    .id = id,
    .type = ak_grestype_shader,
    .ss = ss
  };
  ak_dq_push(&grm->loadings, &li);

  ak_mutex_unlock(&grm->m);
}
