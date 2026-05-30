#include "ak/system/resman.h"
#include "ak/coll/da.h"
#include "ak/coll/hmn.h"
#include "ak/core/async/mutex.h"
#include "ak/core/async/thpool.h"
#include "ak/core/mem/allocator.h"
#include "ak/core/mem/heap.h"
#include "ak/debug.h"
#include "ak/game/stg/world.h"
#include "ak/game/world/write.h"
#include "ak/gfx/img.h"
#include "ak/os/file.h"
#include "ak/system/resman_itn.h"
#include "ak/system/stream.h"

#include <stdint.h>

//===== res_item =====//
//--- private ---//

typedef struct
{
  ak_world world;
  ak_stmerr err;
} res_item_world;

typedef struct
{
  ak_restype type;
  ak_res_status status;
  uint32_t load_count;
  uint32_t access_count;
  ak_alct alct;
  const char* path;
  union
  {
    ak_res_file file;
    ak_res_img img;
    res_item_world wi;
  };
} res_item;

static ak_res_status
status_get_unsafe(ak_resman* rm, ak_resid id)
{
  res_item* ri = ak_hmn_at(&rm->map, id);
  return ri->status;
}

static void
status_set_unsafe(ak_resman* rm,
                  ak_resid id,
                  ak_res_status s)
{
  res_item* ri = ak_hmn_at(&rm->map, id);
  ri->status = s;
}

static res_item
item_get_unsafe(ak_resman* rm, ak_resid id)
{
  res_item ri = { 0 };
  ri = *(res_item*)ak_hmn_at(&rm->map, id);
  return ri;
}

static void
res_load(ak_resman* rm, ak_resid id)
{
  ak_mutex_lock(&rm->m);
  res_item ri = item_get_unsafe(rm, id);
  ak_mutex_unlock(&rm->m);

  ak_restype type = ri.type;
  switch (type) {
    case ak_restype_file: {
      ri.file.size = ak_file_open_read_all(
        ri.path, &ri.file.data, ri.alct);

      ak_mutex_lock(&rm->m);
      res_item* rip =
        ak_hmn_at(&rm->map, id);
      rip->file = ri.file;
      status_set_unsafe(
        rm, id, ak_res_loaded);
      ak_mutex_unlock(&rm->m);
      break;
    }
    case ak_restype_img: {

      ri.img = ak_img_load(ri.path);

      ak_mutex_lock(&rm->m);
      res_item* rip =
        ak_hmn_at(&rm->map, id);
      rip->img = ri.img;
      status_set_unsafe(
        rm, id, ak_res_loaded);
      ak_mutex_unlock(&rm->m);
      break;
    }
    case ak_restype_world: {

      ak_stm stm =
        ak_stm_open_file(ri.path, "rb");
      ri.wi.err = ak_stream_read_world(
        stm, &ri.wi.world, ri.alct);
      ak_stm_close(stm);

      ak_mutex_lock(&rm->m);
      res_item* rip =
        ak_hmn_at(&rm->map, id);
      rip->wi.world = ri.wi.world;
      rip->wi.err = ri.wi.err;
      status_set_unsafe(
        rm, id, ak_res_loaded);
      ak_mutex_unlock(&rm->m);
      break;
    }

    default: {
      ak_assert(false);
      break;
    }
  }
}

static void
res_unload_unsafe(ak_resman* rm, ak_resid id)
{
  res_item* ri = ak_hmn_at(&rm->map, id);
  switch (ri->type) {
    case ak_restype_file: {
      ak_alct_free(ri->alct, ri->file.data);
      ri->file = (ak_res_file){ 0 };
      break;
    }
    case ak_restype_img: {
      ak_img_unload(&ri->img);
      ri->img = (ak_res_img){ 0 };
      break;
    }
    default: {
      ak_assert(false);
      break;
    }
  }
  ri->status = ak_res_not_loaded;
}

//===== ak_resman =====//
//--- private ---//
typedef struct
{
  ak_resman* rm;
  ak_resid id;
} job_in;

static void
job_fn(void* input)
{
  job_in* in = input;
  res_load(in->rm, in->id);
}

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
  item.alct = ak_heap_to_alct(&rm->heap);
  item.path = path;
  item.load_count = 0;
  item.access_count = 0;
  ak_hmn_insert(&rm->map, id, &item);

  ak_mutex_unlock(&rm->m);
}

//--- internal ---//
ak_resman
ak_resman_make(ak_thpool* jp, ak_alct alct)
{
  ak_resman rm = { 0 };
  rm.m = ak_mutex_make();
  rm.heap = ak_heap_make();
  rm.jp = jp;
  rm.map =
    ak_hmn_make(sizeof(res_item), alct);
  rm.jids =
    ak_da_make(sizeof(ak_jobid), alct);
  rm.unloads =
    ak_da_make(sizeof(ak_resid), alct);
  return rm;
}

void
ak_resman_destroy(ak_resman* rm)
{
  ak_assert(false); // todo

  ak_da_destroy(&rm->unloads);
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
    uint32_t count =
      ak_da_count(&rm->unloads);
    uint32_t i = 0;
    while (i < count) {
      ak_resid id = *(ak_resid*)ak_da_at(
        &rm->unloads, i);
      res_item* ri = ak_hmn_at(&rm->map, id);
      if (ri->load_count == 0 &&
          ri->access_count == 0 &&
          ri->status == ak_res_loaded) {
        res_unload_unsafe(rm, id);
        ak_da_remove_swaplast(&rm->unloads,
                              i);
        count--;
      } else {
        i++;
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

void
ak_resman_load(ak_resman* rm, ak_resid id)
{
  ak_mutex_lock(&rm->m);
  res_item* ri = ak_hmn_at(&rm->map, id);

  if (ri->load_count == 0 &&
      ri->status == ak_res_not_loaded) {
    ri->status = ak_res_loading;
    job_in in = { .rm = rm, .id = id };
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
    ak_da_pushback(&rm->unloads, &id);
  }
  ak_mutex_unlock(&rm->m);
}

ak_res_status
ak_resman_status(ak_resman* rm, ak_resid id)
{
  ak_mutex_lock(&rm->m);
  ak_res_status s =
    status_get_unsafe(rm, id);
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
ak_resman_release(ak_resman* rm, ak_resid id)
{
  ak_mutex_lock(&rm->m);
  res_item* ri = ak_hmn_at(&rm->map, id);
  ri->access_count--;
  ak_mutex_unlock(&rm->m);
}

void
ak_resman_register_file(ak_resman* rm,
                        ak_resid id,
                        const char* path)
{
  res_register(
    rm, ak_restype_file, id, path);
}

bool
ak_resman_acquire_file(ak_resman* rm,
                       ak_resid id,
                       ak_res_file* o_file)
{
  ak_assert(ak_resman_res_type(rm, id) ==
            ak_restype_file);

  bool success = false;

  ak_mutex_lock(&rm->m);
  res_item* ri = ak_hmn_at(&rm->map, id);
  if (ri->status == ak_res_loaded) {
    ri->access_count++;
    *o_file = ri->file;
    success = true;
  }
  ak_mutex_unlock(&rm->m);

  return success;
}

void
ak_resman_register_img(ak_resman* rm,
                       ak_resid id,
                       const char* path)
{
  res_register(rm, ak_restype_img, id, path);
}

bool
ak_resman_acquire_img(ak_resman* rm,
                      ak_resid id,
                      ak_res_img* o_img)
{
  ak_assert(ak_resman_res_type(rm, id) ==
            ak_restype_img);

  bool success = false;

  ak_mutex_lock(&rm->m);
  res_item* ri = ak_hmn_at(&rm->map, id);
  if (ri->status == ak_res_loaded) {
    ri->access_count++;
    *o_img = ri->img;
    success = true;
  }
  ak_mutex_unlock(&rm->m);

  return success;
}

void
ak_resman_register_world(ak_resman* rm,
                         ak_resid id,
                         const char* path)
{
  res_register(
    rm, ak_restype_world, id, path);
}

bool
ak_resman_acquire_world(ak_resman* rm,
                        ak_resid id,
                        ak_world* o_world,
                        ak_stmerr* o_err)
{
  ak_assert(ak_resman_res_type(rm, id) ==
            ak_restype_world);

  bool success = false;

  ak_mutex_lock(&rm->m);
  res_item* ri = ak_hmn_at(&rm->map, id);
  if (ri->status == ak_res_loaded) {
    ri->access_count++;
    *o_world = ri->wi.world;
    *o_err = ri->wi.err;
    success = true;
  }
  ak_mutex_unlock(&rm->m);

  return success;
}

/*
ak_mutex_lock(&rm->m);
uint32_t count = ak_da_count(&rm->jids);
for (uint32_t i = 0; i < count; i++) {
ak_jobid jid =
*(ak_jobid*)ak_da_at(&rm->jids, i);
while (ak_thpool_job_status(
     rm->jp, jid) != ak_job_done)
;
ak_thpool_job_remove(rm->jp, jid);
}

ak_hmn_iter it =
ak_hmn_iter_make(&rm->map);
uint64_t key = 0;
void* value = 0;
while (
ak_hmn_iter_next(&it, &key, &value)) {
ak_resid id = key;
res_item* ri = value;

ak_atomicint* ai =
ak_hmn_at(&rm->statuses, id);
while (ak_atomicint_load(ai) ==
   ak_res_loading) {
ak_cpu_yield();
}
if (ak_atomicint_load(ai) ==
ak_res_loaded) {
res_unload(ri);
}
}
ak_mutex_unlock(&rm->m);
*/
