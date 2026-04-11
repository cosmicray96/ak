#include "ak/core/mem/heap.h"
#include "ak/core/errcode.h"
#include "ak/debug.h"
#include <stdlib.h>

//===== ak_heap =====//
//--- export ---//
ak_heap
ak_heap_make()
{
  ak_heap h = { 0 };
  return h;
}

void
ak_heap_destroy(ak_heap* h)
{
  // empty
}

void*
ak_heap_alloc(ak_heap* h, uint32_t size)
{
  void* ptr = malloc(size);
  if (!ptr) {
    ak_ec(ak_err_allocation_failed);
  }
  return ptr;
}

void
ak_heap_free(ak_heap* h, void* ptr)
{
  free(ptr);
}

//===== ak_alct =====//
//--- private ---//
static void*
ak_heap_alloc_b(void* h, uint32_t size)
{
  return ak_heap_alloc((ak_heap*)h, size);
}
static void
ak_heap_free_b(void* h, void* ptr)
{
  ak_heap_free((ak_heap*)h, ptr);
}

//--- export ---//
ak_alct
ak_heap_to_alct(ak_heap* h)
{
  ak_alct a = { 0 };
  a.ctx = h;
  a.alloc = &ak_heap_alloc_b;
  a.free = &ak_heap_free_b;
  return a;
}
