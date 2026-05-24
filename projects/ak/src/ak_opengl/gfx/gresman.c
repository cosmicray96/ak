#include "ak/gfx/gresman.h"
#include "ak/coll/da.h"
#include "ak/coll/hmn.h"
#include "ak/core/async/mutex.h"
#include "ak/core/mem/allocator.h"
#include "ak/debug.h"
#include "ak/gfx/core.h"
#include "ak/system/resman.h"
#include "ak_opengl/gfx/gresman_impl.h"

//===== gres_item =====//
//--- private ---//
typedef struct
{
  ak_gres_status s;
  uint32_t ref_count;
  ak_resid rid;
  GLuint glint;
} gres_item;

//===== ak_gresman =====//
//--- private ---//
struct ak_gresman
{
  ak_alct alct;
  ak_resman* rm;
  ak_gfx* gf;

  ak_mutex m;

  ak_hmn map;
  ak_da loads;
  ak_da unloads;
};

static void
gres_load(ak_gresman* grm, ak_gresid gid)
{
  gres_item* gi = ak_hmn_at(&grm->map, gid);
  ak_res_img* img =
    ak_resman_acquire_img_wait(grm->rm,
                               gi->rid);

  glGenTextures(1, &gi->glint);
  glBindTexture(GL_TEXTURE_2D, gi->glint);

  // upload pixel data
  glTexImage2D(
    GL_TEXTURE_2D,
    0,        // mip level
    GL_RGBA8, // gpu internal format
    img->w,
    img->h,
    0,                // border (must be 0)
    GL_RGBA,          // cpu data format
    GL_UNSIGNED_BYTE, // cpu data type
    img->pixels);

  glBindTexture(GL_TEXTURE_2D, 0);
  gi->s = ak_gres_loaded;
  ak_resman_release(grm->rm, gi->rid);
}

static void
gres_unload(ak_gresman* grm, ak_gresid gid)
{
  gres_item* gi = ak_hmn_at(&grm->map, gid);
  glDeleteTextures(1, &gi->glint);
  gi->s = ak_gres_not_loaded;
}

//--- internal ---//
ak_gresman*
ak_gresman_startup(ak_resman* rm,
                   ak_gfx* gf,
                   ak_alct alct)
{
  ak_gresman* grm =
    ak_alct_alloc(alct, sizeof(ak_gresman));
  grm->alct = alct;
  grm->rm = rm;
  grm->map =
    ak_hmn_make(sizeof(gres_item), alct);
  grm->loads =
    ak_da_make(sizeof(ak_gresid), alct);
  grm->unloads =
    ak_da_make(sizeof(ak_gresid), alct);

  grm->m = ak_mutex_make();
  return grm;
}

void
ak_gresman_shutdown(ak_gresman* grm)
{

  ak_mutex_destroy(&grm->m);

  ak_hmn_iter it =
    ak_hmn_iter_make(&grm->map);
  uint64_t gid = 0;
  gres_item* gi = 0;
  while (ak_hmn_iter_next(
    &it, (&gid), (void**)(&gi))) {
    if (gi->s == ak_gres_loaded) {
      gres_unload(grm, gid);
    }
  }

  ak_da_destroy(&grm->loads);
  ak_hmn_destroy(&grm->map);
  ak_alct_free(grm->alct, grm);
}

void
ak_gresman_register_img(ak_gresman* grm,
                        ak_gresid gid,
                        ak_resid rid)
{
  ak_mutex_lock(&grm->m);
  ak_assert(!ak_hmn_exist(&grm->map, gid));

  gres_item item = { 0 };
  item.s = ak_gres_not_loaded;
  item.rid = rid;
  item.ref_count = 0;
  ak_hmn_insert(&grm->map, gid, &item);

  ak_mutex_unlock(&grm->m);
}

ak_gres_status
ak_gresman_status(ak_gresman* grm,
                  ak_gresid gid)
{

  ak_mutex_lock(&grm->m);
  gres_item* gi = ak_hmn_at(&grm->map, gid);
  ak_gres_status s = gi->s;
  ak_mutex_unlock(&grm->m);
  return s;
}

void
ak_gresman_load(ak_gresman* grm,
                ak_gresid gid)
{

  ak_mutex_lock(&grm->m);

  gres_item* gi = ak_hmn_at(&grm->map, gid);
  ak_gres_status gs = gi->s;

  if (gs == ak_gres_loaded ||
      gs == ak_gres_loading) {
    ak_mutex_unlock(&grm->m);
    return;
  }
  ak_resman_load(grm->rm, gi->rid);
  ak_da_pushback(&grm->loads, &gid);
  gi->s = ak_gres_loading;

  ak_mutex_unlock(&grm->m);
}

void
ak_gresman_release(ak_gresman* grm,
                   ak_gresid gid)
{
  ak_mutex_lock(&grm->m);

  gres_item* gi = ak_hmn_at(&grm->map, gid);
  ak_gres_status s = gi->s;
  if (gi->s == ak_gres_loaded) {
    gi->ref_count--;
    if (gi->ref_count == 0) {
      ak_da_pushback(&grm->unloads, &gid);
    }
  }
  ak_mutex_unlock(&grm->m);
}

void
ak_gresman_update(ak_gresman* grm)
{
  ak_mutex_lock(&grm->m);
  {
    uint32_t count =
      ak_da_count(&grm->unloads);
    for (uint32_t i = 0; i < count; i++) {
      ak_gresid gid = *(ak_gresid*)ak_da_at(
        &grm->unloads, i);
      gres_item* gi =
        ak_hmn_at(&grm->map, gid);
      if (gi->ref_count == 0) {
        gres_unload(grm, gid);
      }
    }
    ak_da_clear(&grm->unloads);
  }

  {
    uint32_t count =
      ak_da_count(&grm->loads);
    uint32_t i = 0;
    while (i < count) {

      ak_gresid gid = *(ak_gresid*)ak_da_at(
        &grm->loads, i);
      gres_item* gi =
        ak_hmn_at(&grm->map, gid);
      ak_res_status rs =
        ak_resman_status(grm->rm, gi->rid);
      if (rs == ak_res_loaded) {
        gres_load(grm, gid);
        ak_da_remove_swaplast(&grm->loads,
                              i);
        count--;
      } else {
        i++;
      }
    }
  }

  ak_mutex_unlock(&grm->m);
}

//--- impl ---//
GLuint
ak_gresman_acquire(ak_gresman* grm,
                   ak_gresid gid)
{
  ak_mutex_lock(&grm->m);
  gres_item* gi = ak_hmn_at(&grm->map, gid);
  ak_assert(gi->s == ak_gres_loaded);
  gi->ref_count++;
  GLuint i = gi->glint;
  ak_mutex_unlock(&grm->m);
  return i;
}
