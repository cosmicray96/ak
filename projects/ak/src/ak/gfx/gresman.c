#include "ak/gfx/gresman.h"
#include "ak/coll/dq.h"
#include "ak/coll/hmn.h"
#include "ak/core/async/mutex.h"
#include "ak/core/mem/allocator.h"
#include "ak/debug.h"
#include "ak/gfx/core.h"
#include "ak/gfx/gresreg.h"
#include "ak/gfx/shader.h"
#include "ak/gfx/tex.h"

//===== gres_item =====//
//--- private ---//

typedef enum
{
  cmdtype_load,
  cmdtype_unload
} cmdtype;

typedef struct
{
  cmdtype type;
  ak_gresid id;
  ak_gresman_args args;
} cmd_item;

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
  ak_dq cmds;
};

static void
gres_load_unsafe(ak_gresman* grm,
                 ak_gresid id,
                 const ak_gresman_args* args)
{
  void* ptr = 0;
  switch (args->type) {
    case ak_grestype_tex: {
      ak_tex* tex =
        ak_tex_make_from_img(grm->gfx,
                             &args->tex.img,
                             args->tex.type,
                             grm->alct);
      ptr = tex;
      break;
    }
    case ak_grestype_shader: {
      ak_shader* shader =
        ak_shader_from_shaderstr(
          grm->gfx, &args->ss, grm->alct);
      ptr = shader;
      break;
    }
    default: {
      ak_assert(false);
    }
  }

  ak_gresreg_reg(
    grm->grr, id, args->type, ptr);
  gres_item* gi = ak_hmn_at(&grm->map, id);
  gi->s = ak_gres_loaded;
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
  grm->cmds =
    ak_dq_make(sizeof(cmd_item), alct);

  grm->m = ak_mutex_make();
  return grm;
}

void
ak_gresman_shutdown(ak_gresman* grm)
{

  ak_mutex_destroy(&grm->m);

  ak_dq_destroy(&grm->cmds);
  ak_hmn_destroy(&grm->map);
  ak_alct_free(grm->alct, grm);
}

void
ak_gresman_update(ak_gresman* grm)
{
  ak_mutex_lock(&grm->m);

  uint32_t budget = 3;
  cmd_item cmd = { 0 };
  while (budget != 0 &&
         ak_dq_pop(&grm->cmds, &cmd)) {

    switch (cmd.type) {
      case cmdtype_load: {
        gres_load_unsafe(
          grm, cmd.id, &cmd.args);
        break;
      }
      case cmdtype_unload: {
        gres_item* gi =
          ak_hmn_at(&grm->map, cmd.id);
        ak_assert(gi->s == ak_gres_loaded);
        if (gi->load_count == 0) {
          gres_unload_unsafe(grm, cmd.id);
        }
        break;
      }
    }

    budget--;
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
    cmd_item cmd = { .type = cmdtype_unload,
                     .id = id };
    ak_dq_push(&grm->cmds, &cmd);
  }

  ak_mutex_unlock(&grm->m);
}

void
ak_gresman_load(ak_gresman* grm,
                ak_gresid id,
                ak_gresman_args args)
{
  ak_mutex_lock(&grm->m);

  if (ak_hmn_exist(&grm->map, id)) {
    gres_item* gi = ak_hmn_at(&grm->map, id);
    gi->load_count++;
    ak_mutex_unlock(&grm->m);
    return;
  }

  gres_item gi = { .type = args.type,
                   .s = ak_gres_loading,
                   .load_count = 1 };
  ak_hmn_insert(&grm->map, id, &gi);

  cmd_item cmd = { .type = cmdtype_load,
                   .id = id,
                   .args = args };
  ak_dq_push(&grm->cmds, &cmd);

  ak_mutex_unlock(&grm->m);
}

void
ak_gresman_load_tex(ak_gresman* grm,
                    ak_gresid id,
                    const ak_img* img,
                    ak_textype textype)
{
  ak_gresman_load(
    grm,
    id,
    (ak_gresman_args){
      .type = ak_grestype_tex,
      .tex = { .img = *img,
               .type = textype } });
}

void
ak_gresman_load_shader(
  ak_gresman* grm,
  ak_gresid id,
  const ak_shaderstr* ss)
{
  ak_gresman_load(
    grm,
    id,
    (ak_gresman_args){ .type =
                         ak_grestype_shader,
                       .ss = *ss });
}
