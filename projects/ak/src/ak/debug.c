#include "ak/debug.h"

//--- private ---//
typedef struct
{
  ak_iostream s;
} debug;
debug d;

//--- public ---//
void
ak_debug_make(ak_iostream s)
{
  d.s = s;
}

void
ak_log_itn(const char* file,
           uint32_t line,
           const char* fmt,
           ...)
{
}

void
ak_ec_itn(const char* file,
          uint32_t line,
          ak_errcode ec)
{
}

void
ak_log_crash(const char* file, uint32_t line)
{
}

void
ak_log_cic()
{
}
