#include "ak/debug.h"
#include "ak/core/io.h"
#include "ak/core/math.h"

#include <stdarg.h>
#include <stdio.h>

//--- private ---//
static const char*
get_filename(const char* path)
{
  const char* last = path;
  const char* p;

  if (!path)
    return 0;

  for (p = path; *p; ++p) {
    if (*p == '/' || *p == '\\')
      last = p + 1;
  }

  return last;
}

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
  char buf[512];
  uint32_t len = 0;
  int32_t n = 0;

  if (len < sizeof(buf) - 1) {
    n = snprintf(buf + len,
                 sizeof(buf) - len,
                 "[Log   ] ");
    ak_assert(n >= 0, ak_err_log_err);
    len = ak_min(len + n, sizeof(buf) - 1);
  }

  if (len < sizeof(buf) - 1) {
    va_list args;
    va_start(args, fmt);
    n = vsnprintf(buf + len,
                  sizeof(buf) - len,
                  fmt,
                  args);
    va_end(args);
    ak_assert(n >= 0, ak_err_log_err);
    len = ak_min(len + n, sizeof(buf) - 1);
  }

  if (len < sizeof(buf) - 1) {
    n = snprintf(buf + len,
                 sizeof(buf) - len,
                 " (%s:%d)\n",
                 get_filename(file),
                 line);
    ak_assert(n >= 0, ak_err_log_err);
    len = ak_min(len + n, sizeof(buf) - 1);
  }

  if (len >= sizeof(buf) - 1) {
    buf[sizeof(buf) - 1] = '\0';
    buf[sizeof(buf) - 2] = '.';
    buf[sizeof(buf) - 3] = '.';
    buf[sizeof(buf) - 4] = '.';
    len = sizeof(buf) - 1;
  }

  ak_iostream_write(d.s, buf, len);
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

ak_ex void
ak_assert_itn(const char* file,
              uint32_t line,
              ak_errcode ec)
{
}
