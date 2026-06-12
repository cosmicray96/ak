#include "ak/res/resman.h"
#include "ak/coll/da.h"
#include "ak/coll/hmn.h"
#include "ak/core/async/mutex.h"
#include "ak/core/async/thpool.h"
#include "ak/core/img.h"
#include "ak/core/mem/allocator.h"
#include "ak/core/mem/heap.h"
#include "ak/core/shaderstr.h"
#include "ak/debug.h"
#include "ak/game/stg/world.h"
#include "ak/game/world/write.h"
#include "ak/os/time.h"
#include "ak/program/program.h"
#include "ak/res/reg.h"
#include "ak/res/resman_itn.h"
#include "ak/system/stream.h"

#include <stdint.h>

//===== res_item =====//
//--- private ---//

typedef struct
{
  ak_resman* rm;
  ak_resid id;
  ak_restype type;
  ak_stm stm;
  ak_alct alct;
} gres_loading_item;

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
  };
} loaded_item;

typedef struct
{
  ak_restype type;
  ak_res_status status;
  ak_stm stm;
  uint32_t load_count;
} res_item;

static void
res_load(ak_resman* rm,
         ak_resid id,
         ak_restype type,
         ak_stm stm,
         ak_alct alct)
{
  loaded_item loaded = { .id = id,
                         .type = type };
  switch (type) {
    case ak_restype_image: {
      ak_img img = ak_img_make_from_stm(
        stm, ak_heap_to_alct(&rm->heap));

      loaded.err = ak_stmerr_ok;
      loaded.img = img;
      break;
    }
    case ak_restype_world: {
      ak_world w = { 0 };
      ak_stmerr err =
        ak_stream_read_world(stm, &w, alct);

      ak_assert(err == ak_stmerr_ok);

      loaded.err = err;
      loaded.world = w;
      break;
    }
    case ak_restype_shaderstr: {
      ak_shaderstr ss = { 0 };
      ak_stmerr err =
        ak_shaderstr_load(stm, &ss, alct);

      ak_assert(err == ak_stmerr_ok);

      loaded.err = err;
      loaded.ss = ss;
      break;
    }

    default: {
      ak_assert(false);
      break;
    }
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

static void
job_fn(void* input)
{
  gres_loading_item* in = input;
  res_load(in->rm,
           in->id,
           in->type,
           in->stm,
           in->alct);
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
  ak_log("Fix Resman");
  ak_this_thread_sleep(ak_dur_from_secs(1));

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
      res_item* ri =
        ak_hmn_at(&rm->map, loaded.id);
      switch (ri->type) {
        case ak_restype_image: {
          ak_resreg_reg_image(
            rm->rr, loaded.id, &loaded.img);
          break;
        }
        case ak_restype_world: {
          ak_resreg_reg_world(rm->rr,
                              loaded.id,
                              &loaded.world);
          break;
        }
        case ak_restype_shaderstr: {
          ak_resreg_reg_shaderstr(
            rm->rr, loaded.id, &loaded.ss);
          break;
        }
        default: {
          ak_assert(false);
        }
      };
      ri->status = ak_res_loaded;
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
               ak_restype type,
               ak_stm stm)
{
  ak_mutex_lock(&rm->m);

  if (ak_hmn_exist(&rm->map, id)) {
    res_item* ri = ak_hmn_at(&rm->map, id);
    ri->load_count++;
    ak_mutex_unlock(&rm->m);
    return;
  }

  res_item ri = { .type = type,
                  .status = ak_res_loading,
                  .stm = stm,
                  .load_count = 1 };
  ak_hmn_insert(&rm->map, id, &ri);

  gres_loading_item in = {
    .rm = rm,
    .id = id,
    .type = type,
    .stm = stm,
    .alct = ak_heap_to_alct(&rm->heap)
  };
  ak_jobid jid = ak_thpool_submit(
    rm->jp,
    &job_fn,
    sizeof(gres_loading_item),
    &in);
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
