#include "ak/core/mem/ptr.h"
#include <string.h>

void
ak_p_cpy(void* dest,
         const void* src,
         uint32_t n)
{
  memcpy(dest, src, n);
}
