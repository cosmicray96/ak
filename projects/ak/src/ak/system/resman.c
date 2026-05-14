#include "ak/system/resman.h"
#include "ak/coll/da.h"
#include "ak/coll/hmn.h"
#include "ak/core/async/atomic.h"
#include "ak/core/async/thpool.h"
#include "ak/core/mem/allocator.h"
#include "ak/core/mem/heap.h"
#include "ak/debug.h"
#include "ak/gfx/img.h"
#include "ak/os/file.h"
#include "ak/system/idgen.h"
#include "ak/system/resman_itn.h"

#include <stdint.h>

//===== res_item =====//
//--- private ---//
typedef struct
{
  ak_atomicint status;
  ak_restype type;
  ak_alct alct;
  const char* path;
  union
  {
    ak_res_file file;
    ak_res_img img;
  };
} res_item;

static ak_resid
resid_make(uint32_t idx, ak_restype type)
{
  uint32_t id = (uint32_t)type << 24;
  id |= idx & 0x00FFFFFF;
  return id;
}

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

static ak_resid
res_register(ak_resman* rm,
             ak_restype type,
             const char* path)
{
  ak_resid id =
    resid_make(ak_idgen_new(&rm->ig), type);

  res_item item = { 0 };
  ak_atomicint_store(&item.status,
                     ak_res_not_loaded);
  item.type = type;
  item.alct = ak_heap_to_alct(&rm->heap);
  item.path = path;
  ak_hmn_insert_u64(&rm->map, id, &item);
  return id;
}

//--- internal ---//
ak_resman
ak_resman_make(ak_thpool* jp, ak_alct alct)
{
  ak_resman rm = { 0 };
  rm.heap = ak_heap_make();
  rm.jp = jp;
  rm.ig = ak_idgen_make(alct);
  rm.map =
    ak_hmn_make(sizeof(res_item), alct);
  rm.jids =
    ak_da_make(sizeof(ak_jobid), alct);
  return rm;
}

void
ak_resman_destroy(ak_resman* rm)
{
  uint32_t count = ak_da_count(&rm->jids);
  for (uint32_t i = 0; i < count; i++) {
    ak_jobid jid = *(ak_jobid*)ak_da_at_impl(
      &rm->jids, i);
    while (ak_thpool_job_status(
             rm->jp, jid) != ak_job_done)
      ;
    ak_thpool_job_remove(rm->jp, jid);
  }

  ak_da_destroy(&rm->jids);
  ak_hmn_destroy(&rm->map);
  ak_idgen_destroy(&rm->ig);
  ak_heap_destroy(&rm->heap);
}

//--- export ---//

ak_resid
ak_resman_register_file(ak_resman* rm,
                        const char* path)
{
  ak_resid id =
    res_register(rm, ak_restype_file, path);
  return id;
}

ak_resid
ak_resman_register_img(ak_resman* rm,
                       const char* path)
{
  ak_resid id =
    res_register(rm, ak_restype_img, path);
  return id;
}

void
ak_resman_load(ak_resman* rm, ak_resid id)
{
  res_item* ri = ak_hmn_at_u64(&rm->map, id);
  ak_res_status s =
    ak_atomicint_load(&ri->status);

  if (s == ak_res_loaded ||
      s == ak_res_loading) {
    return;
  }

  ak_jobid jid =
    ak_thpool_submit(rm->jp, &job_fn, ri);
  ak_da_pushback(&rm->jids, &jid);
}

void
ak_resman_unload(ak_resman* rm, ak_resid id)
{
  res_item* ri = ak_hmn_at_u64(&rm->map, id);
  ak_res_status s =
    ak_atomicint_load(&ri->status);
  if (s != ak_res_loaded) {
    return;
  }
  res_unload(ri);
}

ak_res_status
ak_resman_status(ak_resman* rm, ak_resid id)
{
  res_item* ri = ak_hmn_at_u64(&rm->map, id);
  ak_res_status s =
    ak_atomicint_load(&ri->status);
  return s;
}

ak_restype
ak_resman_res_type(ak_resman* rm,
                   ak_resid rid)
{
  return rid >> 24;
}

ak_res_file*
ak_resman_at_file(ak_resman* rm, ak_resid id)
{
  ak_assert(ak_resman_res_type(rm, id) ==
            ak_restype_file);
  res_item* ri = ak_hmn_at_u64(&rm->map, id);
  ak_res_status s =
    ak_atomicint_load(&ri->status);
  if (s != ak_res_loaded) {
    return 0;
  }
  return &ri->file;
}

ak_res_img*
ak_resman_at_img(ak_resman* rm, ak_resid id)
{
  ak_assert(ak_resman_res_type(rm, id) ==
            ak_restype_img);
  res_item* ri = ak_hmn_at_u64(&rm->map, id);
  ak_res_status s =
    ak_atomicint_load(&ri->status);
  if (s != ak_res_loaded) {
    return 0;
  }
  return &ri->img;
}

void
ak_resman_freejobs(ak_resman* rm)
{
  uint32_t count = ak_da_count(&rm->jids);
  uint32_t i = 0;
  while (i < count) {
    ak_jobid jid = *(ak_jobid*)ak_da_at_impl(
      &rm->jids, i);
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
}
