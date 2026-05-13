#include "ak/core/async/thpool.h"

#include "ak/coll/dq.h"
#include "ak/coll/sla.h"
#include "ak/core/async/atomic.h"
#include "ak/core/async/mutex.h"
#include "ak/core/async/thread.h"
#include "ak/core/mem/heap.h"
#include "ak/debug.h"
#include "ak/os/time.h"

#include <stdlib.h>

//===== ak_job =====//
//--- private ---//
typedef struct
{
  ak_job_fn job;
  void* ctx;
  ak_atomicint status;
} ak_job;

//===== ak_jobpool =====//
//--- private ---//
#define s_thread_count 2

struct ak_thpool
{
  ak_heap heap;
  ak_dq jidq;
  ak_mutex m;
  ak_thread* ts[s_thread_count];
  ak_atomicint shouldclose;
  ak_sla jobs;
};

static void
thpool_job_status_set(ak_thpool* jp,
                      ak_jobid jid,
                      ak_job_status s)
{
  ak_mutex_lock(&jp->m);
  ak_job* j = ak_sla_at(&jp->jobs, jid);
  ak_atomicint_store(&j->status, s);
  ak_mutex_unlock(&jp->m);
}

static bool
thpool_next_job(ak_thpool* jp,
                ak_jobid* o_jid,
                ak_job_fn* o_jfunc,
                void** o_jctx)
{
  ak_mutex_lock(&jp->m);
  bool success = ak_dq_pop(&jp->jidq, o_jid);
  if (success) {
    ak_job* j = ak_sla_at(&jp->jobs, *o_jid);
    *o_jfunc = j->job;
    *o_jctx = j->ctx;
  }
  ak_mutex_unlock(&jp->m);
  return success;
}

static bool
thpool_shouldclose(ak_thpool* jp)
{
  int32_t sc =
    ak_atomicint_load(&jp->shouldclose);
  return sc != 0;
}

static void
thread_fn(void* ctx)
{
  ak_thpool* jp = ctx;
  ak_dur t = ak_dur_from_millis(30);

  while (true) {
    ak_dur start = ak_dur_now();

    ak_jobid jid = 0;
    ak_job_fn jfunc = 0;
    void* jctx = 0;
    bool isjob = thpool_next_job(
      jp, &jid, &jfunc, &jctx);
    if (isjob) {
      thpool_job_status_set(
        jp, jid, ak_job_working);
      jfunc(jctx);
      thpool_job_status_set(
        jp, jid, ak_job_done);
    }

    if (!isjob) {
      if (thpool_shouldclose(jp)) {
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

//--- internal ---//
ak_thpool*
ak_thpool_startup()
{
  ak_thpool* jp = malloc(sizeof(ak_thpool));
  jp->heap = ak_heap_make();

  jp->jidq =
    ak_dq_make(sizeof(ak_jobid),
               10,
               ak_heap_to_alct(&jp->heap));
  jp->jobs =
    ak_sla_make(sizeof(ak_job),
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
ak_thpool_shutdown(ak_thpool* jp)
{
  ak_atomicint_store(&jp->shouldclose, 1);

  for (uint32_t i = 0; i < s_thread_count;
       i++) {
    ak_thread_join(jp->ts[i]);
  }

  ak_mutex_destroy(&jp->m);
  ak_sla_destroy(&jp->jobs);
  ak_dq_destroy(&jp->jidq);
  ak_heap_destroy(&jp->heap);
  free(jp);
}

//--- export ---//
ak_jobid
ak_thpool_submit(ak_thpool* jp,
                 ak_job_fn jfunc,
                 void* jctx)
{
  ak_job j = { 0 };
  j.job = jfunc;
  j.ctx = jctx;
  ak_atomicint_store(&j.status,
                     ak_job_not_started);
  ak_mutex_lock(&jp->m);
  ak_jobid jid =
    ak_sla_insert(&jp->jobs, &j);
  ak_dq_push(&jp->jidq, &jid);
  ak_mutex_unlock(&jp->m);

  return jid;
}

ak_job_status
ak_thpool_job_status(ak_thpool* jp,
                     ak_jobid jid)
{
  ak_mutex_lock(&jp->m);
  ak_job* j = ak_sla_at(&jp->jobs, jid);
  ak_job_status s =
    ak_atomicint_load(&j->status);
  ak_mutex_unlock(&jp->m);

  return s;
}

void
ak_thpool_job_remove(ak_thpool* jp,
                     ak_jobid jid)
{
  ak_mutex_lock(&jp->m);
  ak_job* j = ak_sla_at(&jp->jobs, jid);
  ak_job_status s =
    ak_atomicint_load(&j->status);
  ak_assert(s == ak_job_done);
  ak_sla_remove(&jp->jobs, jid);
  ak_mutex_unlock(&jp->m);
}
