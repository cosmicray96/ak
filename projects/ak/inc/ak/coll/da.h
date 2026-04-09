#ifndef ak_da_h
#define ak_da_h

#include "ak/coll/tbuff.h"
#include <stdint.h>

typedef struct ak_da_impl ak_da_impl;
struct ak_da_impl
{
  ak_tbuff tbuff;
  uint32_t count;
};

#endif
