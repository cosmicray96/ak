#include "ak/core/io.h"
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

//===== ak_iostream =====//
//--- export ---//
void
ak_iostream_write_str(ak_iostream s,
                      const char* str)
{
  uint32_t count = strlen(str);
  s.write(s.ctx, str, count);
}

#define u64_buf_count 32
void
ak_iostream_print_u64(ak_iostream io,
                      uint64_t n)
{
  char buf[u64_buf_count];
  int nn = snprintf(
    buf, u64_buf_count, "%" PRIu64, n);

  if (nn < 0 || nn >= u64_buf_count - 1) {
    ak_iostream_write(io, "Error", 5);
    return;
  }

  ak_iostream_write(io, buf, nn);
}

#define f_buf_count 32
void
ak_iostream_print_d(ak_iostream io, double d)
{
  char buf[f_buf_count];
  int nn =
    snprintf(buf, f_buf_count, "%f", d);

  if (nn < 0 || nn >= f_buf_count - 1) {
    ak_iostream_write(io, "Error", 5);
    return;
  }

  ak_iostream_write(io, buf, nn);
}

//===== sio =====//
//--- private ---//

static int32_t
sio_write(void* ctx,
          const void* buff,
          uint32_t write_count)
{
  size_t written =
    fwrite(buff, 1, write_count, stdout);
  if (written < (size_t)write_count)
    return -1;
  return (int32_t)written;
}

//--- export ---//
ak_iostream
ak_iostream_sio()
{
  ak_iostream s = { 0 };
  s.ctx = 0;
  s.read = 0;
  s.write = &sio_write;
  s.close = 0;

  return s;
}
