#ifndef ak_res_reg_h
#define ak_res_reg_h

#include "ak/core/mem/allocator.h"
#include "ak/game/stg/world.h"
#include "ak/res/reses/img.h"
#include "ak/res/reses/shaderstr.h"

#include <stdbool.h>
#include <stdint.h>

typedef uint32_t ak_resid;

typedef enum
{
  ak_restype_image,
  ak_restype_world,
  ak_restype_shaderstr,
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

ak_img
ak_resreg_get_image(ak_resreg* rr,
                    ak_resid id);
ak_world
ak_resreg_get_world(ak_resreg* rr,
                    ak_resid id);
ak_shaderstr
ak_resreg_get_shaderstr(ak_resreg* rr,
                        ak_resid id);

#endif
