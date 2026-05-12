#ifndef ak_system_resman_itn_h
#define ak_system_resman_itn_h

#include "ak/coll/hmn.h"
#include "ak/core/async/jobpool.h"
#include "ak/core/mem/allocator.h"
#include "ak/export.h"
#include "ak/system/idgen.h"

struct ak_resman
{
  ak_alct alct;
  ak_idgen ig;
  ak_hmn map;
  ak_jobpool* jp;
};

ak_ex ak_resman
ak_resman_make(ak_jobpool* jp, ak_alct alct);
ak_ex void
ak_resman_destroy(ak_resman* rm);

#endif
