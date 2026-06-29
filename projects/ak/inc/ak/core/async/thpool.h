#ifndef ak_core_async_jobpool_h
#define ak_core_async_jobpool_h

#include "ak/coll/da.h"
#include <stdbool.h>
#include <stdint.h>

//===== ak_job =====//
#define ak_s_job_ctx_size 64
typedef uint32_t ak_jobid;
typedef bool (*ak_job_fn)(void* input);
typedef enum
{
  ak_job_none = 0,
  ak_job_not_started = 1,
  ak_job_working = 1 << 1,
  ak_job_done = 1 << 2,
  ak_job_acquired = 1 << 3,
  ak_job_not_exist,
} ak_job_status;

//===== ak_jobpool =====//
typedef struct ak_thpool ak_thpool;

ak_jobid
ak_thpool_submit(ak_thpool* jp,
                 ak_job_fn jfunc,
                 const void* input,
                 uint32_t inputsize,
                 bool job_remove);
ak_job_status
ak_thpool_job_status(ak_thpool* jp,
                     ak_jobid jid);

void
ak_thpool_job_remove(ak_thpool* jp,
                     ak_jobid jid);

void
ak_thpool_clear_done(ak_thpool* jp,
                     ak_da* jids);

typedef struct
{
  ak_job_fn jfunc;
  uint8_t ctx[ak_s_job_ctx_size];
} ak_job_args;
ak_jobid
ak_thpool_submit_array(
  ak_thpool* jp,
  const ak_job_args* jobs,
  uint32_t count,
  bool job_remove);

#endif
