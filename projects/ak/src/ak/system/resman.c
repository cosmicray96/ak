#include "ak/system/resman.h"
#include "ak/coll/hmn.h"
#include "ak/coll/sla.h"
#include "ak/core/async/atomic.h"
#include "ak/core/async/jobpool.h"
#include "ak/system/idgen.h"
#include "ak/system/resman_itn.h"

#include <stdint.h>

//===== res_item =====//
//--- private ---//
typedef struct
{
  ak_atomicint status;
  const char* path;
  void* res;
} res_item;

//===== ak_resman =====//
//--- private ---//
#define s_max_job_count 32

ak_job*
next_free_jobslot(ak_job* jobs)
{
  for (uint32_t i = 0; i < s_max_job_count;
       i++) {
    ak_job_status s =
      ak_job_get_status(&jobs[i]);
    if (s == ak_job_none ||
        s == ak_job_done) {
      return &jobs[i];
    }
  }
  return 0;
}

void
job_fn(void* ctx)
{
  res_item* ri = ctx;
  ak_assert(ak_atomicint_load(&ri->status) ==
            ak_res_not_loaded);
  ak_atomicint_store(&ri->status,
                     ak_res_loading);

  ak_atomicint_store(&ri->status,
                     ak_res_loaded);
}

//--- internal ---//
ak_resman
ak_resman_make(ak_jobpool* jp, ak_alct alct)
{
  ak_resman rm = { 0 };
  rm.alct = alct;
  rm.jp = jp;
  rm.ig = ak_idgen_make(alct);
  rm.map =
    ak_hmn_make(sizeof(res_item), alct);
  rm.jobs = ak_alct_alloc(
    alct, sizeof(ak_job) * s_max_job_count);
  return rm;
}

ak_ex void
ak_resman_destroy(ak_resman* rm);

//--- export ---//
ak_resid
ak_resman_register_file(ak_resman* rm,
                        const char* path)
{
  ak_resid id = ak_idgen_new(&rm->ig);
  res_item item = { 0 };
  ak_atomicint_store(&item.status,
                     ak_res_not_loaded);
  item.res = 0;
  item.path = path;
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

  // ak_jobpool_submit(ak_jobpool *jp, ak_job
  // *j);
}

ak_ex void
ak_resman_unload(ak_resman* rm, ak_resid id);
ak_ex ak_res_status
ak_resman_status(ak_resman* rm, ak_resid id);
ak_ex void*
ak_resman_at(ak_resman* rm, ak_resid id);
