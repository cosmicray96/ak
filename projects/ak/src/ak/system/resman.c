#include "ak/system/resman.h"
#include "ak/coll/da.h"
#include "ak/coll/hmn.h"
#include "ak/core/async/atomic.h"
#include "ak/core/async/thpool.h"
#include "ak/core/mem/heap.h"
#include "ak/debug.h"
#include "ak/os/file.h"
#include "ak/system/idgen.h"
#include "ak/system/resman_itn.h"

#include <stdint.h>

//===== res_item =====//
//--- private ---//
typedef struct
{
  ak_alct alct;
  ak_atomicint status;
  const char* path;
  void* res;
  uint64_t size;
} res_item;

//===== ak_resman =====//
//--- private ---//
static void
job_fn(void* ctx)
{
  res_item* ri = ctx;
  ak_assert(ak_atomicint_load(&ri->status) ==
            ak_res_not_loaded);
  ak_atomicint_store(&ri->status,
                     ak_res_loading);

  ri->size = ak_file_open_read_all(
    ri->path, &ri->res, ri->alct);

  ak_atomicint_store(&ri->status,
                     ak_res_loaded);
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
  ak_resid id = ak_idgen_new(&rm->ig);
  res_item item = { 0 };
  ak_atomicint_store(&item.status,
                     ak_res_not_loaded);
  item.alct = ak_heap_to_alct(&rm->heap);
  item.path = path;
  item.res = 0;
  item.size = 0;
  ak_hmn_insert_u64(&rm->map, id, &item);
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
  ak_alct_free(ri->alct, ri->res);
  ak_atomicint_store(&ri->status,
                     ak_res_not_loaded);
}

ak_res_status
ak_resman_status(ak_resman* rm, ak_resid id)
{
  res_item* ri = ak_hmn_at_u64(&rm->map, id);
  ak_res_status s =
    ak_atomicint_load(&ri->status);
  return s;
}

void*
ak_resman_at(ak_resman* rm,
             ak_resid id,
             uint64_t* o_size)
{
  res_item* ri = ak_hmn_at_u64(&rm->map, id);
  ak_res_status s =
    ak_atomicint_load(&ri->status);
  if (s != ak_res_loaded) {
    return 0;
  }
  if (o_size) {
    *o_size = ri->size;
  }
  return ri->res;
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
