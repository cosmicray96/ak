#include "ak/core/async/jobpool.h"
#include "ak/coll/dq.h"
#include "ak/core/async/atomic.h"
#include "ak/core/async/mutex.h"
#include "ak/core/async/thread.h"
#include "ak/core/mem/heap.h"
#include "ak/os/time.h"

#include <stdlib.h>

//===== ak_job =====//
//--- export ---//
ak_job
ak_job_make(ak_job_fn job, void* ctx)
{
  ak_job j = { 0 };
  j.job = job;
  j.ctx = ctx;
  ak_atomicint_store(&j.status,
                     ak_job_not_started);
  return j;
}

ak_job_status
ak_job_get_status(ak_job* j)
{
  int32_t n = ak_atomicint_load(&j->status);
  return (ak_job_status)n;
}

//===== ak_jobpool =====//
//--- private ---//
#define s_thread_count 2

struct ak_jobpool
{
  ak_heap heap;
  ak_dq jq;
  ak_mutex m;
  ak_thread* ts[s_thread_count];
  bool shouldclose;
};

bool
jobpool_shouldclose(ak_jobpool* jp)
{
  ak_mutex_lock(&jp->m);
  bool shouldclose = jp->shouldclose;
  ak_mutex_unlock(&jp->m);
  return shouldclose;
}

bool
jobpool_next(ak_jobpool* jp, ak_job** o_j)
{
  ak_mutex_lock(&jp->m);
  bool success = ak_dq_pop(&jp->jq, o_j);
  ak_mutex_unlock(&jp->m);
  return success;
}

void
thread_fn(void* ctx)
{
  ak_jobpool* jp = ctx;
  ak_dur t = ak_dur_from_millis(30);

  while (true) {
    ak_dur start = ak_dur_now();

    ak_job* j = 0;
    bool isjob = jobpool_next(jp, &j);
    if (isjob) {
      ak_atomicint_store(&j->status,
                         ak_job_working);
      j->job(j->ctx);
      ak_atomicint_store(&j->status,
                         ak_job_done);
    }

    if (!isjob) {
      if (jobpool_shouldclose(jp)) {
        return;
      }
    }

    ak_dur end = ak_dur_now();
    ak_dur diff =
      ak_dur_subtract(end, start);
    ak_dur sleep_time =
      ak_dur_subtract(t, diff);
    ak_thread_sleep(sleep_time);
  }
}

//--- export ---//
ak_jobpool*
ak_jobpool_startup()
{
  ak_jobpool* jp =
    malloc(sizeof(ak_jobpool));
  jp->heap = ak_heap_make();

  jp->jq =
    ak_dq_make(sizeof(ak_job*),
               10,
               ak_heap_to_alct(&jp->heap));
  jp->m = ak_mutex_make();

  for (uint32_t i = 0; i < s_thread_count;
       i++) {
    jp->ts[i] =
      ak_thread_make(&thread_fn, jp);
  }
  return jp;
}

void
ak_jobpool_shutdown(ak_jobpool* jp)
{
  ak_mutex_lock(&jp->m);
  jp->shouldclose = true;
  ak_mutex_unlock(&jp->m);

  for (uint32_t i = 0; i < s_thread_count;
       i++) {
    ak_thread_join(jp->ts[i]);
  }

  ak_mutex_destroy(&jp->m);
  ak_dq_destroy(&jp->jq);
  ak_heap_destroy(&jp->heap);
  free(jp);
}

void
ak_jobpool_submit(ak_jobpool* jp, ak_job* j)
{
  ak_mutex_lock(&jp->m);
  ak_dq_push(&jp->jq, &j);
  ak_mutex_unlock(&jp->m);
}
