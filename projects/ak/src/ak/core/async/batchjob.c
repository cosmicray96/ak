#include "ak/core/async/batchjob.h"
#include "ak/coll/da.h"
#include "ak/core/async/thpool.h"
#include "ak/os/cpu.h"
#include <stdbool.h>

typedef struct
{
  ak_job_fn jfunc;
  void* jctx;
  ak_jobid jid;
} job_item;

ak_batchjob
ak_batchjob_make(ak_thpool* tp, ak_alct alct)
{
  ak_batchjob bj = { 0 };
  bj.tp = tp;
  bj.jobs =
    ak_da_make(sizeof(job_item), alct);
  return bj;
}

void
ak_batchjob_destroy(ak_batchjob* bj)
{
  ak_da_destroy(&bj->jobs);
  bj->tp = 0;
}

void
ak_batchjob_submit(ak_batchjob* bj,
                   ak_job_fn jfunc,
                   void* jctx)
{
  job_item ji = { 0 };
  ji.jfunc = jfunc;
  ji.jctx = jctx;
  ak_da_pushback(&bj->jobs, &ji);
}

void
ak_batchjob_begin(ak_batchjob* bj)
{
  uint32_t count = ak_da_count(&bj->jobs);
  for (uint32_t i = 0; i < count; i++) {
    job_item* ji =
      ak_da_at_impl(&bj->jobs, i);
    ji->jid = ak_thpool_submit(
      bj->tp, ji->jfunc, ji->jctx);
  }
  uint32_t done = 0;
  while (true) {
    done = 0;

    for (uint32_t i = 0; i < count; i++) {
      job_item* ji =
        ak_da_at_impl(&bj->jobs, i);
      ak_jobid jid = ji->jid;
      if (ak_thpool_job_status(
            bj->tp, jid) == ak_job_done) {
        done++;
      }
    }

    if (done >= count) {
      break;
    }

    if (!ak_thpool_try_run_one(bj->tp)) {
      ak_cpu_yield();
    }
  }

  ak_da_clear(&bj->jobs);
}
