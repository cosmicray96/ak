#ifndef ak_program_event_h
#define ak_program_event_h

#include "ak/export.h"
typedef enum
{
  ak_pgm_none,
  ak_pgm_exit,
} ak_pgm_event;

ak_ex ak_pgm_event
ak_pgm_event_pop();

#endif
