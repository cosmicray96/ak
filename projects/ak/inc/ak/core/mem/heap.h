#ifndef ak_core_mem_heap_h
#define ak_core_mem_heap_h

#include "ak/core/mem/allocator.h"
#include "ak/export.h"

#include <stdint.h>

typedef struct
{
  char ph;
} ak_heap;

ak_ex ak_heap
ak_heap_make();
ak_ex void
ak_heap_destroy(ak_heap* h);

ak_ex void*
ak_heap_alloc(ak_heap* h, uint32_t size);

ak_ex void
ak_heap_free(ak_heap* h, void* ptr);

ak_ex ak_alct
ak_heap_to_alct(ak_heap* h);

#endif
