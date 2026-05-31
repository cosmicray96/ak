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
  float v;
} ak_cnst;

ak_cnst
ak_cnst_make(ak_cnsttype type, float v);

#endif
