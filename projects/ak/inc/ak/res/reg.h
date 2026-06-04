#ifndef ak_res_reg_h
#define ak_res_reg_h

#include "ak/coll/hmn.h"
#include <stdbool.h>
#include <stdint.h>
typedef uint32_t ak_resid;

typedef struct
{
  ak_hmn map;
  uint32_t counter;
} ak_resreg;

ak_resreg
ak_resreg_make(ak_alct alct);
void
ak_resreg_destroy(ak_resreg* rr);

void
ak_resreg_reg_w_id(ak_resreg* rr,
                   ak_resid id,
                   const void* res,
                   uint32_t size);
ak_resid
ak_resreg_reg(ak_resreg* rr,
              const void* res,
              uint32_t size);

void
ak_resreg_unreg(ak_resreg* rr, ak_resid id);

void*
ak_resreg_get(ak_resreg* rr, ak_resid id);

#endif
