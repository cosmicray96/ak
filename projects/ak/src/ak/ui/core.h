#ifndef ak_ui_core_h
#define ak_ui_core_h

#include <float.h>
#include <stdint.h>

typedef uint32_t ak_uiid;

typedef struct
{
  float rel, abs;
} ak_cnst;

static float
ak_cnst_pt(ak_cnst cnst, float pt)
{
  return cnst.rel * pt + cnst.abs;
}

#endif
