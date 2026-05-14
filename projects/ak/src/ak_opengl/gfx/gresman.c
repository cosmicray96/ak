#include "ak/gfx/gresman.h"
#include "ak/coll/da.h"
#include "ak/coll/hmn.h"
#include "ak/core/mem/allocator.h"
#include "ak/debug.h"
#include "ak/system/idgen.h"
#include "ak/system/resman.h"
#include "ak_opengl/gfx/gresman_impl.h"

//===== gres_item =====//
//--- private ---//
typedef struct
{
  ak_gres_status s;
  ak_resid rid;
  GLuint glint;
} gres_item;

//===== ak_gresman =====//
//--- private ---//
struct ak_gresman
{
  ak_alct alct;
  ak_resman* rm;
  ak_idgen ig;
  ak_hmn map;
  ak_da pending;
};

static void
gres_load(ak_gresman* grm, ak_gresid gid)
{
  gres_item* gi =
    ak_hmn_at_u64(&grm->map, gid);
  ak_res_img* img =
    ak_resman_at_img(grm->rm, gi->rid);

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

  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_MIN_FILTER,
                  GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_MAG_FILTER,
                  GL_NEAREST);
  // and probably:
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_WRAP_S,
                  GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_WRAP_T,
                  GL_CLAMP_TO_EDGE);

  glBindTexture(GL_TEXTURE_2D, 0);
  gi->s = ak_gres_loaded;
}

static void
gres_unload(ak_gresman* grm, ak_gresid gid)
{
  gres_item* gi =
    ak_hmn_at_u64(&grm->map, gid);
  glDeleteTextures(1, &gi->glint);
  gi->s = ak_gres_not_loaded;
}

//--- internal ---//
ak_gresman*
ak_gresman_startup(ak_resman* rm,
                   ak_alct alct)
{
  ak_gresman* grm =
    ak_alct_alloc(alct, sizeof(ak_gresman));
  grm->alct = alct;
  grm->rm = rm;
  grm->ig = ak_idgen_make(alct);
  grm->map =
    ak_hmn_make(sizeof(gres_item), alct);
  grm->pending =
    ak_da_make(sizeof(ak_gresid), alct);
  return grm;
}

void
ak_gresman_shutdown(ak_gresman* grm)
{
  ak_hmn_iter it =
    ak_hmn_iter_make(&grm->map);
  uint64_t gid = 0;
  gres_item* gi = 0;
  while (ak_hmn_iter_next_u64(
    &it, (&gid), (void**)(&gi))) {
    if (gi->s == ak_gres_loaded) {
      gres_unload(grm, gid);
    }
  }

  ak_da_destroy(&grm->pending);
  ak_hmn_destroy(&grm->map);
  ak_idgen_destroy(&grm->ig);
  ak_alct_free(grm->alct, grm);
}

ak_gresid
ak_gresman_register_img(ak_gresman* grm,
                        ak_resid rid)
{
  ak_gresid gid = ak_idgen_new(&grm->ig);
  gres_item item = { 0 };
  item.s = ak_gres_not_loaded;
  item.rid = rid;
  ak_hmn_insert_u64(&grm->map, gid, &item);
  return gid;
}

ak_gres_status
ak_gresman_status(ak_gresman* grm,
                  ak_gresid gid)
{
  gres_item* gi =
    ak_hmn_at_u64(&grm->map, gid);
  return gi->s;
}

void
ak_gresman_load(ak_gresman* grm,
                ak_gresid gid)
{
  gres_item* gi =
    ak_hmn_at_u64(&grm->map, gid);
  ak_gres_status gs = gi->s;

  if (gs == ak_gres_loaded ||
      gs == ak_gres_loading) {
    return;
  }
  ak_resman_load(grm->rm, gi->rid);
  ak_da_pushback(&grm->pending, &gid);
  gi->s = ak_gres_loading;
}

void
ak_gresman_unload(ak_gresman* grm,
                  ak_gresid gid)
{
  gres_item* gi =
    ak_hmn_at_u64(&grm->map, gid);
  if (gi->s == ak_gres_loaded) {
    gres_unload(grm, gid);
  }
}

void
ak_gresman_update(ak_gresman* grm)
{
  uint32_t count =
    ak_da_count(&grm->pending);
  uint32_t i = 0;
  while (i < count) {

    ak_gresid gid =
      *(ak_gresid*)ak_da_at_impl(
        &grm->pending, i);
    gres_item* gi =
      ak_hmn_at_u64(&grm->map, gid);
    ak_res_status rs =
      ak_resman_status(grm->rm, gi->rid);

    if (rs == ak_res_loaded) {
      gres_load(grm, gid);
      ak_da_remove_swaplast(&grm->pending,
                            i);
      count--;
    } else {
      i++;
    }
  }
}

//--- impl ---//
GLuint
ak_gresman_at(ak_gresman* grm, ak_gresid gid)
{
  gres_item* gi =
    ak_hmn_at_u64(&grm->map, gid);
  ak_assert(gi->s == ak_gres_loaded);
  return gi->glint;
}
