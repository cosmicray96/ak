#ifndef ak_ui_core_h
#define ak_ui_core_h

#include <float.h>
#include <stdbool.h>
#include <stdint.h>

typedef uint32_t ak_uiid;

typedef struct
{
  float rel, abs;
} ak_cnst;

typedef enum
{
  ak_uiaxis_h = 0,
  ak_uiaxis_v = 1
} ak_uiaxistype;

typedef struct
{
  ak_uiaxistype axistype;
  bool visible;
  bool clipping;

  union
  {
    struct
    {
      ak_cnst x;
      ak_cnst y;
    } cnst;
    ak_cnst cnsts[2];
  };

} ak_uielm_args;

static float
ak_cnst_pt(ak_cnst cnst, float pt)
{
  return cnst.rel * pt + cnst.abs;
}

#endif
