#ifndef ak_core_mem_h
#define ak_core_mem_h

#include <stdint.h>

static uint32_t
ak_align8(uint32_t size)
{
  return (size + 7u) & ~7u;
}

#endif
