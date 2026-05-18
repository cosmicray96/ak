#ifndef ak_core_async_jobpool_h
#define ak_core_async_jobpool_h

#include "ak/export.h"

#include <stdbool.h>
#include <stdint.h>

//===== ak_job =====//
typedef uint32_t ak_jobid;
typedef void (*ak_job_fn)(void* ctx);
typedef enum
{
  ak_job_none,
  ak_job_not_started,
  ak_job_working,
  ak_job_done
} ak_job_status;

//===== ak_jobpool =====//
typedef struct ak_thpool ak_thpool;

ak_jobid
ak_thpool_submit(ak_thpool* jp,
                 ak_job_fn jfunc,
                 void* jctx);

ak_job_status
ak_thpool_job_status(ak_thpool* jp,
                     ak_jobid jid);

void
ak_thpool_job_remove(ak_thpool* jp,
                     ak_jobid jid);

bool
ak_thpool_try_run_one(ak_thpool* tp);

#endif
