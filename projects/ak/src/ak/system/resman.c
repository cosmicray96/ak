#include "ak/system/resman.h"
#include "ak/coll/da.h"
#include "ak/coll/hmn.h"
#include "ak/core/async/atomic.h"
#include "ak/core/async/mutex.h"
#include "ak/core/async/thpool.h"
#include "ak/core/mem/allocator.h"
#include "ak/core/mem/heap.h"
#include "ak/debug.h"
#include "ak/gfx/img.h"
#include "ak/os/cpu.h"
#include "ak/os/file.h"
#include "ak/system/resman_itn.h"

#include <stdint.h>
#include <stdio.h>

//===== res_item =====//
//--- private ---//
typedef struct
{
  ak_atomicint status;
  ak_restype type;
  ak_alct alct;
  uint32_t load_count;
  uint32_t access_count;
  const char* path;
  union
  {
    ak_res_file file;
    ak_res_img img;
  };
} res_item;

static void
res_load(res_item* ri)
{
  ak_atomicint_store(&ri->status,
                     ak_res_loading);
  ak_restype type = ri->type;
  switch (type) {
    case ak_restype_file: {
      ri->file.size = ak_file_open_read_all(
        ri->path, &ri->file.data, ri->alct);
      break;
    }
    case ak_restype_img: {
      ri->img = ak_img_load(ri->path);
      break;
    }
    default: {
      ak_assert(false);
      break;
    }
  }
  ak_atomicint_store(&ri->status,
                     ak_res_loaded);
}

static void
res_unload(res_item* ri)
{
  switch (ri->type) {
    case ak_restype_file: {
      ak_alct_free(ri->alct, ri->file.data);
      break;
    }
    case ak_restype_img: {
      ak_img_unload(&ri->img);
      break;
    }
    default: {
      ak_assert(false);
      break;
    }
  }
  ak_atomicint_store(&ri->status,
                     ak_res_not_loaded);
}

//===== ak_resman =====//
//--- private ---//
static void
job_fn(void* ctx)
{
  res_item* ri = ctx;
  ak_assert(ak_atomicint_load(&ri->status) ==
            ak_res_not_loaded);

  res_load(ri);
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
  ak_atomicint_store(&item.status,
                     ak_res_not_loaded);
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
    while (ak_atomicint_load(&ri->status) ==
           ak_res_loading) {
      ak_cpu_yield();
    }
    if (ak_atomicint_load(&ri->status) ==
        ak_res_loaded) {
      res_unload(ri);
    }
  }

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
      ak_res_status s =
        ak_atomicint_load(&ri->status);
      if (ri->load_count == 0 &&
          ri->access_count == 0 &&
          s == ak_res_loaded) {
        res_unload(ri);
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
ak_resman_register_file(ak_resman* rm,
                        ak_resid id,
                        const char* path)
{
  res_register(
    rm, ak_restype_file, id, path);
}

void
ak_resman_register_img(ak_resman* rm,
                       ak_resid id,
                       const char* path)
{
  res_register(rm, ak_restype_img, id, path);
}

void
ak_resman_load(ak_resman* rm, ak_resid id)
{
  ak_mutex_lock(&rm->m);
  res_item* ri = ak_hmn_at(&rm->map, id);

  if (ri->load_count == 0) {
    ak_jobid jid =
      ak_thpool_submit(rm->jp, &job_fn, ri);
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
  res_item* ri = ak_hmn_at(&rm->map, id);
  ak_res_status s =
    ak_atomicint_load(&ri->status);
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
  ak_res_status s =
    ak_atomicint_load(&ri->status);
  ak_assert(s == ak_res_loaded);
  ri->access_count--;
  ak_mutex_unlock(&rm->m);
}

ak_res_file*
ak_resman_acquire_file(ak_resman* rm,
                       ak_resid id)
{
  ak_assert(ak_resman_res_type(rm, id) ==
            ak_restype_file);

  ak_res_file* file = 0;
  ak_mutex_lock(&rm->m);

  res_item* ri = ak_hmn_at(&rm->map, id);
  ak_res_status s =
    ak_atomicint_load(&ri->status);
  if (s == ak_res_loaded) {
    ri->access_count++;
    file = &ri->file;
  }

  ak_mutex_unlock(&rm->m);
  return file;
}

ak_res_img*
ak_resman_acquire_img(ak_resman* rm,
                      ak_resid id)
{
  ak_assert(ak_resman_res_type(rm, id) ==
            ak_restype_img);

  ak_res_img* img = 0;
  ak_mutex_lock(&rm->m);

  res_item* ri = ak_hmn_at(&rm->map, id);
  ak_res_status s =
    ak_atomicint_load(&ri->status);
  if (s == ak_res_loaded) {
    ri->access_count++;
    img = &ri->img;
  }

  ak_mutex_unlock(&rm->m);
  return img;
}
