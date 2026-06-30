#ifndef ak_res_jobs_h
#define ak_res_jobs_h

#include "ak/res/core.h"
#include "ak/res/reg.h"
#include "ak/res/reses/img.h"
#include "ak/system/stream.h"
#include <stdbool.h>

typedef struct
{
  ak_stm stm;
} ak_job_stmclose;
static bool
ak_job_stmclose_fn(void* ctx)
{
  ak_job_stmclose* ji = ctx;
  ak_stm_close(ji->stm);
  return true;
}

typedef struct
{
  ak_stm stm;
  ak_resreg* rr;
  ak_resid id;
  ak_img* img;
  ak_alct alct;
} ak_job_img_load;
static bool
ak_job_img_load_fn(void* ctx)
{
  ak_job_img_load* ji = ctx;
  ak_stmerr err = ak_stm_read_img(
    ji->stm, ji->img, ji->alct);
  if (err != ak_stmerr_ok) {
    return true;
  }

  ak_resreg_add(ji->rr, ji->id, ji->img);
  return true;
}

typedef struct
{
  ak_img* img;
} ak_job_img_unload;
static bool
ak_job_img_unload_fn(void* ctx)
{
  ak_job_img_unload* ji = ctx;
  ak_img_destroy(ji->img);
  return true;
}

#endif
