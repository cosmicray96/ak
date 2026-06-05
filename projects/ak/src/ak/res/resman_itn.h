#ifndef ak_system_resman_itn_h
#define ak_system_resman_itn_h

#include "ak/coll/da.h"
#include "ak/coll/dq.h"
#include "ak/coll/hmn.h"
#include "ak/core/async/mutex.h"
#include "ak/core/async/thpool.h"
#include "ak/core/mem/allocator.h"
#include "ak/core/mem/heap.h"
#include "ak/export.h"
#include "ak/res/reg.h"
#include "ak/res/resman.h"

struct ak_resman
{
  ak_heap heap;
  ak_resreg* rr;

  ak_mutex m;
  ak_thpool* jp;

  ak_hmn map;
  ak_da jids;

  ak_dq unloads;
  ak_dq loadeds;
};

ak_resman
ak_resman_make(ak_resreg* rr,
               ak_thpool* jp,
               ak_alct alct);
void
ak_resman_destroy(ak_resman* rm);

void
ak_resman_update(ak_resman* rm);

#endif
