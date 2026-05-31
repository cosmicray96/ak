#ifndef ak_ui_core_h
#define ak_ui_core_h

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
    } v;
    float vs[2];
  };
} ak_cnst;

typedef struct
{
  union
  {
    struct
    {
      uint32_t min, max;
    } v;
    uint32_t vs[2];
  };
} ak_uibound;

ak_cnst
ak_cnst_make(ak_cnsttype type,
             float min,
             float max);

#endif
