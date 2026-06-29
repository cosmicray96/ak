#ifndef ak_res_reg_itn_h
#define ak_res_reg_itn_h

#include "ak/coll/hmn.h"
#include "ak/core/async/mutex.h"
#include "ak/res/core.h"

struct ak_resreg
{
  ak_hmn reses;
  ak_mutex m;
};

ak_resreg
ak_resreg_make(ak_alct alct);
void
ak_resreg_destroy(ak_resreg* rr);

#endif
