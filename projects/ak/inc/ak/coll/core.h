#ifndef ak_coll_core_h
#define ak_coll_core_h

#include <stdbool.h>
#include <stdint.h>

typedef bool (*ak_equalfn)(const void* a,
                           const void* b);
typedef uint64_t (*ak_hashfn)(
  const void* key);

static bool
ak_equalfn_ptr(const void* a, const void* b)
{
  return *(const void**)a ==
         *(const void**)b;
}

#endif
