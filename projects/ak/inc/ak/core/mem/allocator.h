#ifndef ak_core_mem_allocator_h
#define ak_core_mem_allocator_h

#include "ak/debug.h"
#include <stdint.h>

typedef struct
{
  void* (*alloc)(void* ctx, uint32_t size);
  void (*free)(void* ctx, void* ptr);
  void* ctx;
} ak_alct;

static void*
ak_alct_alloc(ak_alct a, uint32_t size)
{
  void* p = a.alloc(a.ctx, size);
  // ak_log("Alloced: %p (%d)", p, size);
  return p;
}

static void
ak_alct_free(ak_alct a, void* ptr)
{
  // ak_log("Freed: %p", ptr);
  a.free(a.ctx, ptr);
}

static void
ak_alct_invalidate(ak_alct* a)
{
  a->ctx = 0;
}

#endif
