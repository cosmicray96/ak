#ifndef ak_app_event_h
#define ak_app_event_h

#include "ak/platform/core.h"
#include "ak/program/core.h"

typedef enum
{
  ak_evttype_none,
  ak_evttype_pgm,
  ak_evttype_win,
} ak_evttype;

typedef struct
{
  ak_evttype type;
  union
  {
    ak_evtwin win;
    ak_evtpgm pgm;
  };
} ak_evt;

#endif
