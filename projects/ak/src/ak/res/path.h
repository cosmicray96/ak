#ifndef ak_res_path_h
#define ak_res_path_h

#include "ak/coll/hmn.h"
#include "ak/core/mem/allocator.h"
#include "ak/res/reg.h"

typedef struct
{
  ak_hmn map;
} ak_respathman;

ak_respathman
ak_respathman_make(ak_alct alct);
void
ak_respathman_destroy(ak_respathman* rpm);

void
ak_respathman_reg(ak_respathman* rpm,
                  ak_resid id,
                  ak_restype type,
                  const char* path);

const char*
ak_respathman_get(ak_respathman* rpm,
                  ak_resid id);

#endif
