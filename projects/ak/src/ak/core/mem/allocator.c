#include "ak/core/mem/allocator.h"
#include "ak/core/mem/heap.h"

void*
ak_alct_alloc(ak_alct a, uint32_t size)
{
  switch (a.type) {
    case ak_alcttype_heap: {
      return ak_heap_alloc(a.ctx, size);
    }
  }
}

void
ak_alct_free(ak_alct a, void* ptr)
{
  switch (a.type) {
    case ak_alcttype_heap: {
      ak_heap_free(a.ctx, ptr);
      break;
    }
  }
}
