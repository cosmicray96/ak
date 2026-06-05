#include "ak/res/resman.h"
#include "ak/coll/da.h"
#include "ak/coll/hmn.h"
#include "ak/core/async/mutex.h"
#include "ak/core/async/thpool.h"
#include "ak/core/img.h"
#include "ak/core/mem/allocator.h"
#include "ak/core/mem/heap.h"
#include "ak/debug.h"
#include "ak/game/stg/world.h"
#include "ak/game/world/write.h"
#include "ak/os/cpu.h"
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
  const char* path;
  ak_alct alct;
} job_in;

typedef struct
{
  ak_resid id;
  ak_restype type;
  ak_stmerr err;
  union
  {
    ak_img img;
    ak_world world;
  };
} loaded_item;

typedef struct
{
  ak_restype type;
  ak_res_status status;
  const char* path;
  uint32_t load_count;

} res_item;

static void
res_load(ak_resman* rm,
         ak_resid id,
         ak_restype type,
         const char* path,
         ak_alct alct)
{
  loaded_item loaded = { .id = id,
                         .type = type };
  switch (type) {
    case ak_restype_image: {
      ak_img img =
        ak_img_make_from_path(path);

      loaded.err = ak_stmerr_ok;
      loaded.img = img;
      break;
    }
    case ak_restype_world: {
      ak_stm stm =
        ak_stm_open_file(path, "rb");
      ak_world w = { 0 };
      ak_stmerr err =
        ak_stream_read_world(stm, &w, alct);
      ak_stm_close(stm);

      loaded.err = err;
      loaded.world = w;
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
  job_in* in = input;
  res_load(in->rm,
           in->id,
           in->type,
           in->path,
           in->alct);
}
//===== ak_resman =====//
//--- private ---//

static void
res_register(ak_resman* rm,
             ak_restype type,
             ak_resid id,
             const char* path)
{
  ak_mutex_lock(&rm->m);

  ak_assert(!ak_hmn_exist(&rm->map, id));

  res_item item = { 0 };
  item.status = ak_res_not_loaded;
  item.type = type;
  item.path = path;
  item.load_count = 0;
  ak_hmn_insert(&rm->map, id, &item);

  ak_mutex_unlock(&rm->m);
}

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

  ak_mutex_lock(&rm->m);
  ak_hmn_iter it =
    ak_hmn_iter_make(&rm->map);
  uint64_t key = 0;
  void* value = 0;
  while (
    ak_hmn_iter_next(&it, &key, &value)) {
    ak_resid id = key;
    res_item* ri = value;
    if (ri->status == ak_res_not_loaded) {
      continue;
    }
    while (ri->status == ak_res_loading) {
      ak_cpu_yield();
    }
    res_unload_unsafe(rm, id);
  }
  ak_mutex_unlock(&rm->m);

  ak_dq_destroy(&rm->loadeds);
  ak_dq_destroy(&rm->unloads);
  ak_da_destroy(&rm->jids);
  ak_hmn_destroy(&rm->map);
  ak_heap_destroy(&rm->heap);
  ak_mutex_destroy(&rm->m);
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
      ak_resreg_reg(rm->rr,
                    loaded.id,
                    loaded.type,
                    &loaded.img);
      ri->status = ak_res_loaded;
    }
  }

  {
    ak_resid id = { 0 };
    while (ak_dq_pop(&rm->unloads, &id)) {
      res_item* ri = ak_hmn_at(&rm->map, id);
      if (ri->load_count) {
        continue;
      }
      res_unload_unsafe(rm, id);
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

void
ak_resman_load(ak_resman* rm, ak_resid id)
{
  ak_mutex_lock(&rm->m);
  res_item* ri = ak_hmn_at(&rm->map, id);

  if (ri->load_count == 0 &&
      ri->status == ak_res_not_loaded) {
    ri->status = ak_res_loading;
    job_in in = { .rm = rm,
                  .id = id,
                  .type = ri->type,
                  .path = ri->path,
                  .alct = ak_heap_to_alct(
                    &rm->heap) };
    ak_jobid jid = ak_thpool_submit(
      rm->jp, &job_fn, sizeof(job_in), &in);
    ak_da_pushback(&rm->jids, &jid);
  }
  ri->load_count++;
  ak_mutex_unlock(&rm->m);
}

void
ak_resman_unload(ak_resman* rm, ak_resid id)
{
  ak_mutex_lock(&rm->m);
  res_item* ri = ak_hmn_at(&rm->map, id);

  ak_assert(ri->load_count != 0);
  ri->load_count--;
  if (ri->load_count == 0) {
    ak_dq_push(&rm->unloads, &id);
  }
  ak_mutex_unlock(&rm->m);
}

ak_res_status
ak_resman_status(ak_resman* rm, ak_resid id)
{
  ak_mutex_lock(&rm->m);
  res_item* ri = ak_hmn_at(&rm->map, id);
  ak_res_status s = ri->status;
  ak_mutex_unlock(&rm->m);
  return s;
}

ak_restype
ak_resman_res_type(ak_resman* rm,
                   ak_resid rid)
{
  ak_mutex_lock(&rm->m);
  res_item* ri = ak_hmn_at(&rm->map, rid);
  ak_restype type = ri->type;
  ak_mutex_unlock(&rm->m);
  return type;
}

void
ak_resman_register_img(ak_resman* rm,
                       ak_resid id,
                       const char* path)
{
  res_register(
    rm, ak_restype_image, id, path);
}

void
ak_resman_register_world(ak_resman* rm,
                         ak_resid id,
                         const char* path)
{
  res_register(
    rm, ak_restype_world, id, path);
}
