#ifndef ak_ui_core_h
#define ak_ui_core_h

#include "ak/debug.h"
#include <float.h>
#include <stdint.h>

typedef uint32_t ak_uiid;

typedef enum
{
  ak_cnsttype_abs,
  ak_cnsttype_rel,
} ak_cnsttype;

typedef struct
{
  ak_cnsttype type;
  union
  {
    struct
    {
      float min, max;
    };
    float vs[2];
  };
} ak_cnst;

typedef struct
{
  union
  {
    struct
    {
      float min, max;
    };
    float vs[2];
  };
} ak_uibound;

ak_cnst
ak_cnst_make(ak_cnsttype type,
             float min,
             float max)
{
  ak_assert(type == ak_cnsttype_abs ||
            type == ak_cnsttype_rel);
  ak_assert(min >= 0);
  ak_assert(max >= 0);
  ak_assert(min <= max);
  if (type == ak_cnsttype_rel) {
    ak_assert(min <= 1);
    ak_assert(max <= 1);
  }
  return (ak_cnst){ .type = type,
                    .min = min,
                    .max = max };
}

static ak_uibound
ak_uibound_make(float min, float max)
{
  ak_assert(min <= max);
  return (ak_uibound){ .min = min,
                       .max = max };
}

static ak_uibound
ak_uibound_make_biggest()
{
  return (ak_uibound){ .min = 0,
                       .max = FLT_MAX };
}

ak_uibound
ak_uibound_from_cnst(ak_cnst cnst)
{
  if (cnst.type == ak_cnsttype_rel) {
    return ak_uibound_make_biggest();
  }
  return ak_uibound_make(cnst.min, cnst.max);
}

ak_uibound
ak_uibound_from_cnst_pt(ak_cnst cnst,
                        float size_pt)
{
  if (cnst.type == ak_cnsttype_abs) {
    size_pt = 1;
  }
  return ak_uibound_make(cnst.min * size_pt,
                         cnst.max * size_pt);
}

#endif
