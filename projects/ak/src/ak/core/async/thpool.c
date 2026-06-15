#include "ak/core/async/thpool.h"

#include "ak/coll/dq.h"
#include "ak/coll/sla.h"
#include "ak/core/async/atomic.h"
#include "ak/core/async/mutex.h"
#include "ak/core/async/thread.h"
#include "ak/core/mem/heap.h"
#include "ak/core/mem/ptr.h"
#include "ak/debug.h"
#include "ak/os/time.h"

#include <stdlib.h>

//===== ak_job =====//
#define ak_s_job_ctx_size 64
//--- private ---//
typedef struct
{
  ak_job_fn job;
  uint32_t input_size;
  uint8_t input[ak_s_job_ctx_size];
  ak_job_status status;
  bool autoremove;
} job_item;

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
status_set(ak_thpool* jp,
           ak_jobid jid,
           ak_job_status status)
{
  ak_mutex_lock(&jp->m);
  job_item* ji = ak_sla_at(&jp->jobs, jid);
  ji->status = status;
  ak_mutex_unlock(&jp->m);
}

static ak_job_status
status_get(ak_thpool* jp, ak_jobid jid)
{
  ak_mutex_lock(&jp->m);
  if (!ak_sla_exist(&jp->jobs, jid)) {
    ak_mutex_unlock(&jp->m);
    return ak_job_not_exist;
  }
  job_item* ji = ak_sla_at(&jp->jobs, jid);
  ak_job_status s = ji->status;
  ak_mutex_unlock(&jp->m);
  return s;
}

static bool
thpool_next_job(
  ak_thpool* jp,
  ak_jobid* o_jid,
  ak_job_fn* o_jobfn,
  uint8_t o_input[ak_s_job_ctx_size],
  bool* o_autoremove)
{
  ak_mutex_lock(&jp->m);
  bool success = ak_dq_pop(&jp->jidq, o_jid);
  if (success) {
    job_item* ji =
      ak_sla_at(&jp->jobs, *o_jid);
    *o_jobfn = ji->job;
    ak_p_cpy(
      o_input, ji->input, ji->input_size);
    *o_autoremove = ji->autoremove;
  }
  ak_mutex_unlock(&jp->m);
  return success;
}

static void
thpool_repushback(ak_thpool* jp,
                  ak_jobid id,
                  const void* input)
{
  ak_mutex_lock(&jp->m);
  job_item* ji = ak_sla_at(&jp->jobs, id);
  ak_p_cpy(ji->input, input, ji->input_size);
  ak_dq_push(&jp->jidq, &id);
  ak_mutex_unlock(&jp->m);
}

static void
thread_fn(void* ctx)
{
  ak_thpool* jp = ctx;
  ak_dur t = ak_dur_from_millis(30);

  while (true) {
    ak_dur start = ak_dur_now();

    ak_jobid id = 0;
    ak_job_fn jobfn = 0;
    uint8_t input[ak_s_job_ctx_size] = { 0 };
    bool autoremove = false;
    bool isjob = thpool_next_job(
      jp, &id, &jobfn, input, &autoremove);
    if (isjob) {
      bool finished = jobfn(input);
      if (finished) {
        status_set(jp, id, ak_job_done);
        if (autoremove) {
          ak_thpool_job_remove(jp, id);
        }
      } else {
        thpool_repushback(jp, id, input);
      }
    }

    if (!isjob) {
      if (ak_atomicint_load(
            &jp->shouldclose)) {
        return;
      }
    }

    ak_dur end = ak_dur_now();
    ak_dur diff =
      ak_dur_subtract(end, start);
    ak_dur sleep_time =
      ak_dur_subtract(t, diff);
    ak_this_thread_sleep(sleep_time);
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
               ak_heap_to_alct(&jp->heap));
  jp->jobs =
    ak_sla_make(sizeof(job_item),
                ak_heap_to_alct(&jp->heap));
  jp->m = ak_mutex_make();

  ak_atomicint_store(&jp->shouldclose, 0);

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
                 uint32_t inputsize,
                 const void* input,
                 bool job_remove)
{
  ak_assert(inputsize <= ak_s_job_ctx_size);

  job_item j = { 0 };
  j.job = jfunc;
  j.input_size = inputsize;
  j.status = ak_job_working;
  j.autoremove = job_remove;
  ak_p_cpy(&j.input, input, inputsize);

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
  return status_get(jp, jid);
}

void
ak_thpool_job_remove(ak_thpool* jp,
                     ak_jobid jid)
{
  ak_assert(ak_thpool_job_status(jp, jid) ==
            ak_job_done);

  ak_mutex_lock(&jp->m);
  ak_sla_remove(&jp->jobs, jid);
  ak_mutex_unlock(&jp->m);
}

void
ak_thpool_clear_done(ak_thpool* jp,
                     ak_da* jids)
{
  uint32_t count = ak_da_count(jids);
  uint32_t i = 0;
  while (i < count) {
    ak_jobid jid =
      *(ak_jobid*)ak_da_at(jids, i);
    ak_job_status s =
      ak_thpool_job_status(jp, jid);
    if (s == ak_job_done) {
      ak_thpool_job_remove(jp, jid);
      ak_da_remove_swaplast(jids, i);
      count--;
    } else {
      i++;
    }
  }
}

/*
bool
ak_thpool_try_run_one(ak_thpool* tp)
{
  ak_jobid jid = 0;
  ak_job_fn jfunc = 0;
  void* jctx = 0;
  bool isjob =
    thpool_next_job(tp, &jid, &jfunc, &jctx);
  if (isjob) {
    thpool_job_status_set(
      tp, jid, ak_job_working);
    jfunc(jctx);
    thpool_job_status_set(
      tp, jid, ak_job_done);
  }
  return isjob;
}
*/
