#ifndef ak_core_async_jobpool_h
#define ak_core_async_jobpool_h

#include "ak/core/async/atomic.h"
#include "ak/export.h"

//===== ak_job =====//
typedef void (*ak_job_fn)(void* ctx);
typedef enum
{
  ak_job_not_started,
  ak_job_working,
  ak_job_done
} ak_job_status;
typedef struct
{
  ak_job_fn job;
  void* ctx;
  ak_atomicint status;
} ak_job;

ak_ex ak_job
ak_job_make(ak_job_fn job, void* ctx);
ak_ex ak_job_status
ak_job_get_status(ak_job* j);

//===== ak_jobpool =====//
typedef struct ak_jobpool ak_jobpool;
ak_ex ak_jobpool*
ak_jobpool_startup();
ak_ex void
ak_jobpool_shutdown(ak_jobpool* jp);

ak_ex void
ak_jobpool_submit(ak_jobpool* jp, ak_job* j);

#endif
