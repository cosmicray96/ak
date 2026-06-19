#include "ak/res/resman.h"
#include "ak/coll/da.h"
#include "ak/coll/hmn.h"
#include "ak/core/async/mutex.h"
#include "ak/core/async/thpool.h"
#include "ak/core/mem/allocator.h"
#include "ak/core/mem/heap.h"
#include "ak/debug.h"
#include "ak/game/stg/world.h"
#include "ak/game/world/write.h"
#include "ak/res/reg.h"
#include "ak/res/reses/aniclip.h"
#include "ak/res/reses/texatlas.h"
#include "ak/res/resman_itn.h"
#include "ak/system/stream.h"

#include <stdint.h>

//===== res_item =====//
//--- private ---//

typedef struct
{
  ak_resman* rm;
  ak_resid id;
  ak_resman_args args;
} args_item;

typedef struct
{
  ak_resid id;
  ak_restype type;
  ak_stmerr err;
  union
  {
    ak_img img;
    ak_world world;
    ak_shaderstr ss;
    ak_texatlas ta;
    ak_aniclip ac;
  };
} loaded_item;

typedef struct
{
  ak_restype type;
  ak_res_status status;
  uint32_t load_count;
} res_item;

static void
res_load(const args_item* ai)
{
  ak_resman* rm = ai->rm;
  ak_resid id = ai->id;
  ak_restype type = ai->args.type;
  ak_stm stm = ai->args.stm;
  ak_alct alct = ak_heap_to_alct(&rm->heap);
  bool stm_close = ai->args.stm_close;

  loaded_item loaded = { .id = id,
                         .type = type };
  switch (type) {
    case ak_restype_image: {
      ak_img img =
        ak_img_make_from_stm(stm, alct);

      loaded.err = ak_stmerr_ok;
      loaded.img = img;
      break;
    }
    case ak_restype_world: {
      ak_world w = { 0 };
      ak_stmerr err =
        ak_stream_read_world(stm, &w, alct);

      loaded.err = err;
      loaded.world = w;
      break;
    }
    case ak_restype_shaderstr: {
      ak_shaderstr ss = { 0 };
      ak_stmerr err = ak_stm_read_shaderstr(
        stm, &ss, alct);

      loaded.err = err;
      loaded.ss = ss;
      break;
    }
    case ak_restype_texatlas: {
      ak_texatlas ta = { 0 };
      ak_stmerr err =
        ak_stm_read_texatlas(stm, &ta, alct);

      loaded.err = err;
      loaded.ta = ta;
      break;
    }
    case ak_restype_aniclip: {
      ak_aniclip ac = { 0 };
      ak_stmerr err = ak_stm_read_aniclip(
        stm, ai->args.rid, &ac, alct);

      loaded.err = err;
      loaded.ac = ac;
      break;
    }

    default: {
      ak_assert(false);
      break;
    }
  }
  if (loaded.err != ak_stmerr_ok) {
    ak_log("stmerr, %d", loaded.err);
  }
  if (stm_close) {
    ak_stm_close(stm);
  }
  ak_mutex_lock(&rm->m);
  ak_dq_push(&rm->loadeds, &loaded);
  ak_mutex_unlock(&rm->m);
}

static void
res_unload_unsafe(ak_resman* rm, ak_resid id)
{
  res_item* ri = ak_hmn_at(&rm->map, id);
  ak_resreg_unreg(rm->rr, id);
  ri->status = ak_res_not_loaded;
}

static bool
job_fn(void* input)
{
  res_load(input);
  return true;
}
//===== ak_resman =====//
//--- private ---//

//--- internal ---//
ak_resman
ak_resman_make(ak_resreg* rr,
               ak_thpool* jp,
               ak_alct alct)
{
  ak_resman rm = { 0 };
  rm.rr = rr;
  rm.m = ak_mutex_make();
  rm.heap = ak_heap_make();
  rm.jp = jp;
  rm.map =
    ak_hmn_make(sizeof(res_item), alct);
  rm.jids =
    ak_da_make(sizeof(ak_jobid), alct);
  rm.unloads =
    ak_dq_make(sizeof(ak_resid), alct);
  rm.loadeds =
    ak_dq_make(sizeof(loaded_item), alct);
  return rm;
}

void
ak_resman_destroy(ak_resman* rm)
{
  ak_mutex_lock(&rm->m);
  uint32_t loadeds_count =
    ak_dq_count(&rm->loadeds);
  uint32_t unloads_count =
    ak_dq_count(&rm->unloads);
  ak_mutex_unlock(&rm->m);

  if (loadeds_count != 0 ||
      unloads_count != 0) {
    ak_resman_update(rm);
  }

  ak_dq_destroy(&rm->loadeds);
  ak_dq_destroy(&rm->unloads);
  ak_da_destroy(&rm->jids);
  ak_hmn_destroy(&rm->map);
  ak_heap_destroy(&rm->heap);
  ak_mutex_destroy(&rm->m);
}

ak_resreg*
ak_resman_resreg(ak_resman* rm)
{
  return rm->rr;
}

void
ak_resman_update(ak_resman* rm)
{
  ak_mutex_lock(&rm->m);

  {
    loaded_item loaded = { 0 };
    while (
      ak_dq_pop(&rm->loadeds, &loaded)) {
      if (loaded.err == ak_stmerr_ok) {
        res_item* ri =
          ak_hmn_at(&rm->map, loaded.id);
        ak_resreg_reg(rm->rr,
                      loaded.id,
                      loaded.type,
                      &loaded.img);
        ri->status = ak_res_loaded;
      }
    }
  }

  {
    ak_resid id = { 0 };
    while (ak_dq_pop(&rm->unloads, &id)) {
      res_item* ri = ak_hmn_at(&rm->map, id);
      if (ri->load_count == 0) {
        res_unload_unsafe(rm, id);
        ak_hmn_remove(&rm->map, id);
      }
    }
  }

  uint32_t count = ak_da_count(&rm->jids);
  uint32_t i = 0;
  while (i < count) {
    ak_jobid jid =
      *(ak_jobid*)ak_da_at(&rm->jids, i);
    ak_job_status s =
      ak_thpool_job_status(rm->jp, jid);
    if (s == ak_job_done) {
      ak_thpool_job_remove(rm->jp, jid);
      ak_da_remove_swaplast(&rm->jids, i);
      count--;
    } else {
      i++;
    }
  }
  ak_mutex_unlock(&rm->m);
}

//--- export ---//

ak_res_status
ak_resman_status(ak_resman* rm, ak_resid id)
{
  ak_mutex_lock(&rm->m);
  if (!ak_hmn_exist(&rm->map, id)) {
    ak_mutex_unlock(&rm->m);
    return ak_res_not_exist;
  }
  res_item* ri = ak_hmn_at(&rm->map, id);
  ak_res_status s = ri->status;
  ak_mutex_unlock(&rm->m);
  return s;
}

void
ak_resman_load(ak_resman* rm,
               ak_resid id,
               const ak_resman_args* args)
{
  ak_mutex_lock(&rm->m);

  if (ak_hmn_exist(&rm->map, id)) {
    res_item* ri = ak_hmn_at(&rm->map, id);
    ri->load_count++;
    ak_mutex_unlock(&rm->m);
    return;
  }

  res_item ri = { .type = args->type,
                  .status = ak_res_loading,
                  .load_count = 1 };
  ak_hmn_insert(&rm->map, id, &ri);

  args_item ai = { .rm = rm,
                   .id = id,
                   .args = *args };
  ak_jobid jid =
    ak_thpool_submit(rm->jp,
                     &job_fn,
                     &ai,
                     sizeof(args_item),
                     false);
  ak_da_pushback(&rm->jids, &jid);

  ak_mutex_unlock(&rm->m);
}

void
ak_resman_unload(ak_resman* rm, ak_resid id)
{
  ak_mutex_lock(&rm->m);

  res_item* ri = ak_hmn_at(&rm->map, id);

  ak_assert(ri->load_count > 0);

  ri->load_count--;
  if (ri->load_count == 0) {
    ak_dq_push(&rm->unloads, &id);
  }

  ak_mutex_unlock(&rm->m);
}
