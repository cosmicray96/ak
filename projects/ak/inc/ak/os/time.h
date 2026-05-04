#ifndef ak_os_time_h
#define ak_os_time_h

#include "ak/export.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct
{
  uint64_t nanos;
} ak_dur;

ak_ex ak_dur
ak_dur_now();

ak_ex ak_dur
ak_dur_from_secs(uint64_t secs);
ak_ex ak_dur
ak_dur_from_millis(uint64_t millis);
ak_ex ak_dur
ak_dur_from_micros(uint64_t micros);
ak_ex ak_dur
ak_dur_from_nanos(uint64_t nanos);

ak_ex ak_dur
ak_dur_subtract(ak_dur a, ak_dur b);
ak_ex ak_dur
ak_dur_diff(ak_dur a, ak_dur b);
ak_ex ak_dur
ak_dur_add(ak_dur a, ak_dur b);
ak_ex bool
ak_dur_zero(ak_dur a);

ak_ex float
ak_dur_as_secs_f(ak_dur d);

#endif
