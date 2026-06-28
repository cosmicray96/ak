#include "ak/app/core.h"
#include "ak/os/time.h"

//--- private ---//
static ak_dur s_delta = { 0 };

//--- export ---//
ak_dur
ak_delta()
{
  return s_delta;
}

void
ak_delta_set(ak_dur delta)
{
  s_delta = delta;
}
