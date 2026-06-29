#ifndef ak_res_core_h
#define ak_res_core_h

#include <stdint.h>

typedef uint32_t ak_resid;
typedef struct ak_resreg ak_resreg;

typedef struct
{
  void (*get)(void* ctx);
  void (*destroy)(void* ctx);
} ak_resvt;

typedef struct
{
  const ak_resvt* vt;
  void* ctx;
} ak_res;

#endif
