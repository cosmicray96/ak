#include "ak/core/mem/ptr.h"
#include <stdint.h>
#include <string.h>

void
ak_p_cpy(void* dest,
         const void* src,
         uint32_t count)
{
  memcpy(dest, src, count);
}

ak_ex void
ak_p_set_byte(void* dest,
              uint8_t n,
              uint32_t count)
{
  memset(dest, n, count);
}

void
ak_p_swap(void* p1, void* p2, uint32_t count)
{
  uint8_t* b1 = p1;
  uint8_t* b2 = p2;
  for (size_t i = 0; i < count; i++) {
    uint8_t tmp = b1[i];
    b1[i] = b2[i];
    b2[i] = tmp;
  }
}
