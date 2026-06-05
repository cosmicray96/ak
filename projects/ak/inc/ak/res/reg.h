#ifndef ak_res_reg_h
#define ak_res_reg_h

#include "ak/core/mem/allocator.h"

#include <stdbool.h>
#include <stdint.h>

typedef uint32_t ak_resid;

typedef enum
{
  ak_restype_image,
  ak_restype_world,
  // ak_restype_animation,
  // ak_restype_aniclip,
  ak_restype_count,
} ak_restype;

typedef struct ak_resreg ak_resreg;

ak_resreg*
ak_resreg_make(ak_alct alct);
void
ak_resreg_destroy(ak_resreg* rr);

void
ak_resreg_reg(ak_resreg* rr,
              ak_resid id,
              ak_restype type,
              const void* res);

void
ak_resreg_unreg(ak_resreg* rr, ak_resid id);

void
ak_resreg_get(ak_resreg* rr,
              ak_resid id,
              void* o_res);

#endif
