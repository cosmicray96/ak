#include "ak/core/io.h"
#include <string.h>

void
ak_iostream_write_str(ak_iostream s,
                      const char* str)
{
  uint32_t count = strlen(str);
  s.write(s.ctx, str, count);
}
