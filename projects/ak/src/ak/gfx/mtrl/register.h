#ifndef ak_gfx_mtrl_register_h
#define ak_gfx_mtrl_register_h

#include "ak/coll/hmn.h"
#include "ak/core/mem/allocator.h"
#include "ak/gfx/core.h"
#include <stdint.h>

typedef struct
{
  ak_hmn map;
} ak_mtrlreg;

ak_mtrlreg
ak_mtrlreg_make(ak_alct alct);
void
ak_mtrlreg_destroy(ak_mtrlreg* mr);

void
ak_mtrlreg_insert(ak_mtrlreg* mr,
                  ak_mtrltype mt,
                  void* mtrl);
void*
ak_mtrlreg_at(ak_mtrlreg* mr,
              ak_mtrltype mt);

#endif
