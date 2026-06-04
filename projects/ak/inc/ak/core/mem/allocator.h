#ifndef ak_core_mem_allocator_h
#define ak_core_mem_allocator_h

#include "ak/debug.h"
#include "ak/export.h"
#include <stdint.h>

typedef enum
{
  ak_alcttype_heap,
} ak_alcttype;

typedef struct
{
  ak_alcttype type;
  void* ctx;
} ak_alct;

ak_ex void*
ak_alct_alloc(ak_alct a, uint32_t size);

ak_ex void
ak_alct_free(ak_alct a, void* ptr);

static void
ak_alct_invalidate(ak_alct* a)
{
  a->ctx = 0;
}

#endif
