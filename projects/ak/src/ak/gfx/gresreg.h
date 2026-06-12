#ifndef ak_gfx_greg_h
#define ak_gfx_greg_h

#include "ak/core/mem/allocator.h"
#include "ak/gfx/core.h"
#include "ak/gfx/tex.h"

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
  ak_grestype_tex,
  ak_grestype_shader,
  ak_grestype_count,
} ak_grestype;

typedef struct ak_gresreg ak_gresreg;

ak_gresreg*
ak_gresreg_make(ak_gfx* gfx, ak_alct alct);
void
ak_gresreg_destroy(ak_gresreg* grr);

void
ak_gresreg_reg(ak_gresreg* grr,
               ak_gresid id,
               ak_grestype type,
               void* gres);
void
ak_gresreg_unreg(ak_gresreg* grr,
                 ak_gresid id);
void*
ak_gresreg_get(ak_gresreg* grr,
               ak_gresid id);

void
ak_gresreg_reg_tex(ak_gresreg* grr,
                   ak_gresid id,
                   ak_tex* tex);
ak_tex*
ak_gresreg_get_tex(ak_gresreg* grr,
                   ak_gresid id);

void
ak_gresreg_reg_shader(ak_gresreg* grr,
                      ak_gresid id,
                      ak_shader* shader);
ak_shader*
ak_gresreg_get_shader(ak_gresreg* grr,
                      ak_gresid id);

#endif
