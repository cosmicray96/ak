#ifndef ak_coll_fcnst_h
#define ak_coll_fcnst_h

#include "ak/coll/spa.h"
#include "ak/core/mem/allocator.h"
#include <stdbool.h>
typedef struct
{
  ak_spa spa;
  uint32_t root;
} ak_fcnst;

ak_fcnst
ak_fcnst_make(ak_alct alct);
void
ak_fcnst_destroy(ak_fcnst* t);

void
ak_fcnst_add(ak_fcnst* t,
             uint32_t id,
             uint32_t ptid);

typedef void (*ak_fcnst_remove_fn)(
  void* ctx,
  uint32_t id);
void
ak_fcnst_remove(ak_fcnst* t,
                uint32_t id,
                ak_fcnst_remove_fn remove_fn,
                void* ctx);

uint32_t
ak_fcnst_root(ak_fcnst* t);
bool
ak_fcnst_exist(ak_fcnst* t, uint32_t id);

uint32_t
ak_fcnst_pt(ak_fcnst* t, uint32_t id);
uint32_t
ak_fcnst_fc(ak_fcnst* t, uint32_t id);
uint32_t
ak_fcnst_ns(ak_fcnst* t, uint32_t id);
uint32_t
ak_fcnst_ps(ak_fcnst* t, uint32_t id);

#endif
