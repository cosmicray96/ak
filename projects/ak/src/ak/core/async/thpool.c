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
#define ak_s_timeout_secs 2
#define s_thread_count 2
//--- private ---//
typedef struct
{
  ak_job_fn jobfn;
  uint8_t input[ak_s_job_ctx_size];
  bool autoremove;
  ak_jobid next;
} job;
typedef struct
{
  ak_job_status s;
  job j;
} job_item;

//===== ak_jobpool =====//
//--- private ---//

struct ak_thpool
{
  ak_heap heap;
  ak_dq jidq;
  ak_mutex m;
  ak_thread* ts[s_thread_count];
  ak_atomicint shouldclose;
  ak_sla jobs;
  ak_dur destroy_start;
};

static void
status_set(ak_thpool* jp,
           ak_jobid jid,
           ak_job_status status)
{
  ak_mutex_lock(&jp->m);
  job_item* ji = ak_sla_at(&jp->jobs, jid);
  ji->s = status;
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
  ak_job_status s = ji->s;
  ak_mutex_unlock(&jp->m);
  return s;
}

static bool
job_next(ak_thpool* jp,
         ak_jobid* o_jid,
         job* o_job)
{
  ak_mutex_lock(&jp->m);
  bool success = ak_dq_pop(&jp->jidq, o_jid);
  if (success) {
    job_item* ji =
      ak_sla_at(&jp->jobs, *o_jid);
    *o_job = ji->j;
  }
  ak_mutex_unlock(&jp->m);
  return success;
}

static void
repushback(ak_thpool* jp,
           ak_jobid id,
           const job* job)
{
  ak_mutex_lock(&jp->m);
  job_item* ji = ak_sla_at(&jp->jobs, id);
  ji->j = *job;
  ak_dq_push(&jp->jidq, &id);
  ak_mutex_unlock(&jp->m);
}

static void
job_push(ak_thpool* jp, ak_jobid id)
{
  ak_mutex_lock(&jp->m);
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

    bool shouldclose =
      ak_atomicint_load(&jp->shouldclose);

    ak_jobid id = 0;
    job job = { 0 };
    bool isjob = job_next(jp, &id, &job);
    if (isjob) {
      bool finished = job.jobfn(job.input);
      if (finished) {
        status_set(jp, id, ak_job_done);
        if (job.autoremove) {
          ak_thpool_job_remove(jp, id);
        }
        if (job.next) {
          job_push(jp, job.next);
        }
      } else {
        repushback(jp, id, &job);
      }
    }

    if (shouldclose) {

      if (!isjob) {
        return;
      }

      ak_dur start = jp->destroy_start;
      ak_dur end = ak_dur_now();
      ak_dur diff =
        ak_dur_subtract(end, start);
      ak_dur timeout =
        ak_dur_from_secs(ak_s_timeout_secs);
      if (ak_dur_gt(diff, timeout)) {
        ak_log("thpool. timeout reached, "
               "force destroy.");
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

static void
add_dummy(ak_thpool* tp)
{
  uint32_t idx =
    ak_sla_insert_empty(&tp->jobs);
  ak_assert(idx == 0);
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
  jp->destroy_start = ak_dur_now();

  ak_atomicint_store(&jp->shouldclose, 0);

  add_dummy(jp);

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
  ak_mutex_destroy(&jp->m);
  ak_sla_destroy(&jp->jobs);
  ak_dq_destroy(&jp->jidq);
  ak_heap_destroy(&jp->heap);
  free(jp);
}

void
ak_thpool_destroy_jobs(ak_thpool* jp)
{
  jp->destroy_start = ak_dur_now();
  ak_atomicint_store(&jp->shouldclose, 1);

  for (uint32_t i = 0; i < s_thread_count;
       i++) {
    ak_thread_join(jp->ts[i]);
  }
}

//--- export ---//

ak_jobid
ak_thpool_submit(ak_thpool* jp,
                 ak_job_fn jobfn,
                 const void* input,
                 uint32_t inputsize,
                 bool autoremove)
{
  ak_assert(inputsize <= ak_s_job_ctx_size);

  job_item ji = { .s = ak_job_working,
                  .j = { .jobfn = jobfn,
                         .autoremove =
                           autoremove,
                         .next = 0 } };
  ak_p_cpy(&ji.j.input, input, inputsize);

  ak_mutex_lock(&jp->m);
  ak_jobid jid =
    ak_sla_insert(&jp->jobs, &ji);
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

ak_jobid
ak_thpool_submit_array(
  ak_thpool* jp,
  const ak_job_args* jobs,
  uint32_t count,
  bool autoremove)
{
  ak_jobid first_id = 0;
  ak_jobid jid = 0;
  ak_mutex_lock(&jp->m);

  for (uint32_t i = 0; i < count; i++) {
    const ak_job_args* job_arg = &jobs[i];

    job_item ji = {
      .s = ak_job_working,
      .j = { .jobfn = job_arg->jfunc,
             .autoremove = i == count - 1
                             ? autoremove
                             : true,
             .next = 0 }
    };
    ak_p_cpy(&ji.j.input,
             job_arg->ctx,
             ak_s_job_ctx_size);

    ak_jobid last_id = jid;
    jid = ak_sla_insert(&jp->jobs, &ji);

    if (i == 0) {
      first_id = jid;
    } else {
      job_item* last_ji =
        ak_sla_at(&jp->jobs, last_id);
      last_ji->j.next = jid;
    }
  }
  ak_dq_push(&jp->jidq, &first_id);
  ak_mutex_unlock(&jp->m);

  return jid;
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
