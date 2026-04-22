#ifndef ak_core_mem_ptr_h
#define ak_core_mem_ptr_h

#include "ak/export.h"
#include <stdint.h>

static void*
ak_p_add(const void* p, uint32_t n)
{
  return (void*)((uintptr_t)p + n);
}
static void*
ak_p_sub(const void* p, uint32_t n)
{
  return (void*)((uintptr_t)p - n);
}

ak_ex void
ak_p_cpy(void* dest,
         const void* src,
         uint32_t n);

#endif
