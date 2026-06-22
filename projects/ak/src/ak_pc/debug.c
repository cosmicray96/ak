#include "ak/debug_itn.h"
#include <stdio.h>

void
ak_debug_log_impl(const char* log)
{
  printf("%s", log);
}
