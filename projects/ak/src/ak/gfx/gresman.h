#ifndef ak_gfx_gresman_h
#define ak_gfx_gresman_h

#include "ak/core/img.h"
#include "ak/core/mem/allocator.h"
#include "ak/core/shaderstr.h"
#include "ak/gfx/core.h"
#include "ak/gfx/gresreg.h"
#include "ak/gfx/tex.h"
#include <stdint.h>

typedef enum
{
  ak_gres_not_exist,
  ak_gres_not_loaded,
  ak_gres_loading,
  ak_gres_loaded,
} ak_gres_status;

typedef struct ak_gresman ak_gresman;
ak_gresman*
ak_gresman_startup(ak_gresreg* grr,
                   ak_gfx* gfx,
                   ak_alct alct);
void
ak_gresman_shutdown(ak_gresman* grm);

void
ak_gresman_update(ak_gresman* grm);

ak_gres_status
ak_gresman_status(ak_gresman* grm,
                  ak_gresid id);

void
ak_gresman_unload(ak_gresman* grm,
                  ak_gresid id);

void
ak_gresman_load_tex(ak_gresman* grm,
                    ak_gresid id,
                    ak_img img,
                    ak_textype type);

void
ak_gresman_load_shader(ak_gresman* grm,
                       ak_gresid id,
                       ak_shaderstr ss);

#endif
