#include "ak/debug.h"
#include "ak/core/errcode.h"
#include "ak/core/io.h"
#include "ak/debug_itn.h"
#include "ak/os/file.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

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

//===== fmter =====//
//--- private ---//
typedef struct
{
  char* buf;
  uint32_t size;
  uint32_t len;
  int32_t last_n;
  bool full;
} fmter;

static fmter
fmter_make(char* buf, uint32_t size)
{
  ak_assert(buf);
  ak_assert(size > 5);

  fmter f = { 0 };
  f.buf = buf;
  f.size = size;
  f.last_n = 0;
  f.len = 0;
  f.full = false;
  return f;
}

static void
fmter_full_itn(fmter* f)
{
  if (f->full) {
    return;
  }
  f->full = true;

  f->buf[f->size - 1] = '\0';
  f->buf[f->size - 2] = '\n';
  f->buf[f->size - 3] = '.';
  f->buf[f->size - 4] = '.';
  f->buf[f->size - 5] = '.';
  f->len = f->size - 1;
}
static void
fmter_update_itn(fmter* f)
{
  ak_assert(f->last_n >= 0);
  if (f->len + f->last_n >= f->size) {
    fmter_full_itn(f);
  } else {
    f->len += f->last_n;
  }
  f->last_n = 0;
}

static void
fmter_str(fmter* f, const char* str)
{
  if (f->full) {
    return;
  }
  f->last_n = snprintf(f->buf + f->len,
                       f->size - f->len,
                       "%s",
                       str);
  fmter_update_itn(f);
}

static void
fmter_valist(fmter* f,
             const char* fmt,
             va_list args)
{
  if (f->full) {
    return;
  }

  f->last_n = vsnprintf(f->buf + f->len,
                        f->size - f->len,
                        fmt,
                        args);
  fmter_update_itn(f);
}

static void
fmter_vargs(fmter* f, const char* fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  fmter_valist(f, fmt, args);
  va_end(args);
}

static char*
fmter_buf(fmter* f)
{
  return f->buf;
}
static uint32_t
fmter_len(fmter* f)
{
  return f->len;
}

//===== debug =====//
//--- private ---//
typedef struct
{
  ak_iostream s;
  ak_iostream sraw;
} debug;
debug d = { 0 };

//--- public ---//
void
ak_debug_startup()
{
  d.s = ak_fstream_make("./log.txt", "w");

  d.sraw =
    ak_frawstream_make("./lograw.txt", "w");

  const char* buf = "Debug Startup.\n";
  ak_iostream_write(d.s, buf, strlen(buf));
}

void
ak_debug_shutdown()
{
  const char* buf = "Debug Shutdown.\n";
  ak_iostream_write(d.s, buf, strlen(buf));
  ak_iostream_close(&d.s);
}

//--- export ---//
void
ak_log_itn(const char* file,
           uint32_t line,
           const char* fmt,
           ...)
{
  char buf[256];
  fmter f = fmter_make(buf, sizeof(buf));

  fmter_str(&f, "[Log] ");

  va_list args;
  va_start(args, fmt);
  fmter_valist(&f, fmt, args);
  va_end(args);

  fmter_vargs(&f,
              " (%s:%d)\n",
              get_filename(file),
              line);

  ak_iostream_write(
    d.s, fmter_buf(&f), fmter_len(&f));
}

void
ak_ec_itn(const char* file,
          uint32_t line,
          ak_errcode ec)
{
  char buf[256];
  fmter f = fmter_make(buf, sizeof(buf));

  fmter_str(&f, "[Err] ");

  fmter_vargs(&f,
              "%s (%s:%d)\n",
              ak_errcode_to_str(ec),
              get_filename(file),
              line);

  ak_iostream_write(
    d.s, fmter_buf(&f), fmter_len(&f));
}

void
ak_assert_itn(const char* file,
              uint32_t line,
              const char* expr)
{
  char buf[256];
  fmter f = fmter_make(buf, sizeof(buf));

  fmter_str(&f, "[Ast] ");

  fmter_vargs(&f,
              "((%s)) (%s:%d)\n",
              expr,
              get_filename(file),
              line);

  ak_iostream_write(
    d.s, fmter_buf(&f), fmter_len(&f));
}

void
ak_log_cic()
{
  char buf[256];
  fmter f = fmter_make(buf, sizeof(buf));

  fmter_str(&f, "[Cic]\n");

  ak_iostream_write(
    d.s, fmter_buf(&f), fmter_len(&f));
}
void
ak_log_crash()
{
  char buf[256];
  fmter f = fmter_make(buf, sizeof(buf));

  fmter_str(&f, "[Crh]\n");

  ak_iostream_write(
    d.s, fmter_buf(&f), fmter_len(&f));
}

#define s_fatal_log_size 7
void
ak_log_crash_fatal()
{
  char buf[s_fatal_log_size] = "[Ftl]\n";
  ak_iostream_write(
    d.sraw, buf, s_fatal_log_size);
}
