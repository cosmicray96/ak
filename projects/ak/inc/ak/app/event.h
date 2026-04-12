#ifndef ak_app_event_h
#define ak_app_event_h

#include "ak/platform/core.h"

typedef enum
{
  ak_evt_none,
  ak_evt_type_win,
} ak_evt_type;

typedef struct
{
  ak_evt_type type;
  union
  {
    ak_winevt win;
  };
} ak_evt;

#endif
