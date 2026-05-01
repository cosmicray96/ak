#include "ak/core/math/math.h"
#include "ak/core/math/itn.h"

void
ak_math_startup()
{
  ak_sin_lut_init();
}

void
ak_math_shutdown()
{
  // empty
}
