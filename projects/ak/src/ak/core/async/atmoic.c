#include "ak/core/async/atomic.h"

//===== ak_atomicint =====//
//--- export ---//
ak_atomicint
ak_atomicint_make(int32_t v)
{
  ak_atomicint ai = { 0 };
  ai.v = v;
  return ai;
}

#if defined(__GNUC__) || defined(__clang__)

void
ak_atomicint_store(ak_atomicint* ai,
                   int32_t v)
{
  __atomic_store_n(
    &ai->v, v, __ATOMIC_RELEASE);
}
int32_t
ak_atomicint_load(ak_atomicint* ai)
{
  return __atomic_load_n(&ai->v,
                         __ATOMIC_ACQUIRE);
}

#elif defined(_WIN32) || defined(__CYGWIN__)

#include <windows.h>

void
ak_atomicint_store(ak_atomicint* ai,
                   int32_t v)
{
  InterlockedExchange((LONG*)&ai->v, v);
}

int32_t
ak_atomicint_load(ak_atomicint* ai)
{
  return InterlockedAdd((LONG*)&ai->v, 0);
}

#else

#error "unknown compiler"

#endif
