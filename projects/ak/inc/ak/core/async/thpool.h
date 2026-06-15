#ifndef ak_core_async_jobpool_h
#define ak_core_async_jobpool_h

#include "ak/coll/da.h"
#include <stdbool.h>
#include <stdint.h>

//===== ak_job =====//
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
                 uint32_t inputsize,
                 const void* input,
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

#endif
