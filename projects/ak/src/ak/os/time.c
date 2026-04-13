#include "ak/os/time.h"
#include <stdbool.h>

//===== ak_dur =====//

//--- export ---//
ak_dur
ak_dur_from_secs(uint64_t secs)
{
  ak_dur d = { 0 };
  d.nanos = secs * 1000000000ULL;
  return d;
}

ak_dur
ak_dur_from_millis(uint64_t millis)
{
  ak_dur d = { 0 };
  d.nanos = millis * 1000000ULL;
  return d;
}

ak_dur
ak_dur_from_micros(uint64_t micros)
{

  ak_dur d = { 0 };
  d.nanos = micros * 1000ULL;
  return d;
}

ak_dur
ak_dur_from_nanos(uint64_t nanos)
{

  ak_dur d = { 0 };
  d.nanos = nanos;
  return d;
}

ak_dur
ak_dur_subtract(ak_dur a, ak_dur b)
{
  ak_dur res = { 0 };
  if (a.nanos < b.nanos) {
    res.nanos = 0;
    return res;
  }
  res.nanos = a.nanos - b.nanos;
  return res;
}

ak_dur
ak_dur_diff(ak_dur a, ak_dur b)
{
  ak_dur res = { 0 };
  if (a.nanos < b.nanos) {
    res.nanos = b.nanos - a.nanos;
    return res;
  }
  res.nanos = a.nanos - b.nanos;
  return res;
}

ak_dur
ak_dur_add(ak_dur a, ak_dur b)
{
  ak_dur res = { 0 };
  res.nanos = a.nanos + b.nanos;
  return res;
}

bool
ak_dur_zero(ak_dur a)
{
  return a.nanos == 0;
}
