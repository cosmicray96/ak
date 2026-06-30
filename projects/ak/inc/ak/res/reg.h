#ifndef ak_res_reg_h
#define ak_res_reg_h

#include "ak/res/core.h"

void*
ak_resreg_get(ak_resreg* rr, ak_resid id);

void
ak_resreg_add(ak_resreg* rr,
              ak_resid id,
              void* res);
void
ak_resreg_remove(ak_resreg* rr, ak_resid id);

#endif
