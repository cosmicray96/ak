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
  ak_grestype type;
  ak_gres_status s;
  uint32_t access_count;
  uint32_t load_count;
  union
  {
    struct
    {
      ak_resid rid;
      GLuint glint;
    } img;
    struct
    {
      GLuint glint;
    } fb;
  };
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
gres_load_unsafe(ak_gresman* grm,
                 ak_gresid gid)
{
  gres_item* gi = ak_hmn_at(&grm->map, gid);
  switch (gi->type) {
    case ak_grestype_tex: {
      ak_res_img img = { 0 };
      bool success = ak_resman_acquire_img(
        grm->rm, gi->img.rid, &img);

      glGenTextures(1, &gi->img.glint);
      glBindTexture(GL_TEXTURE_2D,
                    gi->img.glint);

      glTexImage2D(GL_TEXTURE_2D,
                   0,
                   GL_RGBA8,
                   img.w,
                   img.h,
                   0,
                   GL_RGBA,
                   GL_UNSIGNED_BYTE,
                   img.pixels);

      glBindTexture(GL_TEXTURE_2D, 0);
      gi->s = ak_gres_loaded;
      ak_resman_release(grm->rm,
                        gi->img.rid);
      ak_resman_unload(grm->rm, gi->img.rid);
      break;
    }
    default: {
      ak_assert(false);
    }
  }
}

static void
gres_unload_unsafe(ak_gresman* grm,
                   ak_gresid gid)
{
  gres_item* gi = ak_hmn_at(&grm->map, gid);
  ak_assert(gi->s == ak_gres_loaded);
  switch (gi->type) {
    case ak_grestype_tex: {
      glDeleteTextures(1, &gi->img.glint);
      gi->s = ak_gres_not_loaded;
      break;
    }
    default: {
      ak_assert(false);
    }
  }
}

static void
gres_register(ak_gresman* grm,
              ak_grestype type,
              ak_gresid gid)
{
  ak_assert(!ak_hmn_exist(&grm->map, gid));

  gres_item item = { 0 };
  item.type = type;
  item.s = ak_gres_not_loaded;
  item.load_count = 0;
  item.access_count = 0;
  ak_hmn_insert(&grm->map, gid, &item);
}

//--- impl ---//
bool
ak_gresman_acquire_tex(ak_gresman* grm,
                       ak_gresid gid,
                       GLuint* o_glint)
{
  ak_mutex_lock(&grm->m);
  gres_item* gi = ak_hmn_at(&grm->map, gid);
  if (gi->s == ak_gres_loaded) {
    gi->access_count++;
    *o_glint = gi->img.glint;
    ak_mutex_unlock(&grm->m);
    return true;
  }
  ak_mutex_unlock(&grm->m);
  return false;
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
      gres_unload_unsafe(grm, gid);
    }
  }

  ak_da_destroy(&grm->loads);
  ak_hmn_destroy(&grm->map);
  ak_alct_free(grm->alct, grm);
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

      if (gi->load_count == 0 &&
          gi->access_count == 0 &&
          gi->s == ak_gres_loaded) {
        gres_unload_unsafe(grm, gid);
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

      switch (gi->type) {
        case ak_grestype_tex: {
          ak_res_status rs =
            ak_resman_status(grm->rm,
                             gi->img.rid);
          if (rs == ak_res_loaded) {
            gres_load_unsafe(grm, gid);
            ak_da_remove_swaplast(
              &grm->loads, i);
            count--;
          } else {
            i++;
          }
          break;
        }
        default: {
          ak_assert(false);
        }
      }
    }
  }

  ak_mutex_unlock(&grm->m);
}

//--- export ---//
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

ak_grestype
ak_gresman_type(ak_gresman* grm,
                ak_gresid gid)
{
  ak_mutex_lock(&grm->m);
  gres_item* gi = ak_hmn_at(&grm->map, gid);
  ak_grestype type = gi->type;
  ak_mutex_unlock(&grm->m);
  return type;
}

void
ak_gresman_register_tex_from_rid(
  ak_gresman* grm,
  ak_gresid gid,
  ak_resid rid)
{
  ak_mutex_lock(&grm->m);
  gres_register(grm, ak_grestype_tex, gid);

  gres_item* item =
    ak_hmn_at(&grm->map, gid);
  item->img.rid = rid;

  ak_mutex_unlock(&grm->m);
}

void
ak_gresman_register_framebuffer(
  ak_gresman* grm,
  ak_gresid gid);

void
ak_gresman_load(ak_gresman* grm,
                ak_gresid gid)
{
  ak_mutex_lock(&grm->m);

  gres_item* gi = ak_hmn_at(&grm->map, gid);
  if (gi->load_count == 0 &&
      gi->s == ak_gres_not_loaded) {
    gi->s = ak_gres_loading;

    switch (gi->type) {
      case ak_grestype_tex: {
        ak_resman_load(grm->rm, gi->img.rid);
        break;
      }
      default: {
        ak_assert(false);
      }
    }

    ak_da_pushback(&grm->loads, &gid);
  }
  gi->load_count++;

  ak_mutex_unlock(&grm->m);
}

void
ak_gresman_unload(ak_gresman* grm,
                  ak_gresid gid)
{
  ak_mutex_lock(&grm->m);

  gres_item* gi = ak_hmn_at(&grm->map, gid);
  ak_resman_unload(grm->rm, gi->img.rid);
  gi->load_count--;
  if (gi->load_count == 0) {
    ak_da_pushback(&grm->unloads, &gid);
  }

  ak_mutex_unlock(&grm->m);
}

void
ak_gresman_release(ak_gresman* grm,
                   ak_gresid gid)
{
  ak_mutex_lock(&grm->m);

  gres_item* gi = ak_hmn_at(&grm->map, gid);
  ak_gres_status s = gi->s;
  gi->access_count--;
  if (gi->access_count == 0) {
    ak_da_pushback(&grm->unloads, &gid);
  }
  ak_mutex_unlock(&grm->m);
}
