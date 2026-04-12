#ifndef ak_app_eq_h
#define ak_app_eq_h

#include "ak/app/event.h"
#include "ak/coll/dq.h"
#include "ak/core/mem/allocator.h"

typedef struct
{
  ak_dq events;
} ak_app_eq;

ak_app_eq
ak_app_eq_make(ak_alct alct);
ak_app_eq
ak_app_eq_destroy(ak_app_eq* eq);

void
ak_app_eq_push(ak_app_eq* eq, ak_evt e);
ak_evt
ak_app_eq_pop(ak_app_eq* eq);

#endif
