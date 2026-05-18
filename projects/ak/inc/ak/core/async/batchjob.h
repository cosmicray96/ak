#ifndef ak_core_async_batchjob_h
#define ak_core_async_batchjob_h

#include "ak/coll/da.h"
#include "ak/core/async/thpool.h"
#include "ak/core/mem/allocator.h"
typedef struct
{
  ak_thpool* tp;
  ak_da jobs;
} ak_batchjob;

ak_batchjob
ak_batchjob_make(ak_thpool* tp,
                 ak_alct alct);
void
ak_batchjob_destroy(ak_batchjob* bj);

void
ak_batchjob_submit(ak_batchjob* bj,
                   ak_job_fn jfunc,
                   void* jctx);

void
ak_batchjob_begin(ak_batchjob* bj);

#endif
