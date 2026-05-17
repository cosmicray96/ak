#include "ak/core/io.h"
#include "ak/debug.h"

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
  fflush(stdout);
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

void
ak_write_bool(ak_iostream s, bool n)
{
  uint8_t b = n ? 1 : 0;
  ak_iostream_write(s, &b, 1);
}

bool
ak_read_bool(ak_iostream s)
{
  bool b = 0;
  int32_t c = ak_iostream_read(s, &b, 1);
  ak_assert(c == sizeof(bool));
  return b;
}

void
ak_write_i16(ak_iostream s, int16_t n)
{
  uint8_t b[2];
  b[0] = (uint8_t)(n);
  b[1] = (uint8_t)(n >> 8);
  ak_iostream_write(s, b, 2);
}

int16_t
ak_read_i16(ak_iostream s)
{
  uint8_t b[2];
  int32_t c = ak_iostream_read(s, b, 2);
  ak_assert(c == 2);
  return (int16_t)((uint16_t)(b[0]) |
                   (uint16_t)(b[1]) << 8);
}

void
ak_write_u16(ak_iostream s, uint16_t n)
{
  uint8_t b[2];
  b[0] = (uint8_t)(n);
  b[1] = (uint8_t)(n >> 8);
  ak_iostream_write(s, b, 2);
}

uint16_t
ak_read_u16(ak_iostream s)
{
  uint8_t b[2];
  int32_t c = ak_iostream_read(s, b, 2);
  ak_assert(c == 2);
  return (uint16_t)(b[0]) | (uint16_t)(b[1])
                              << 8;
}

void
ak_write_i32(ak_iostream s, int32_t n)
{
  uint8_t b[4];
  b[0] = (uint8_t)(n);
  b[1] = (uint8_t)(n >> 8);
  b[2] = (uint8_t)(n >> 16);
  b[3] = (uint8_t)(n >> 24);
  ak_iostream_write(s, b, 4);
}

int32_t
ak_read_i32(ak_iostream s)
{
  uint8_t b[4];
  int32_t c = ak_iostream_read(s, b, 4);
  ak_assert(c == 4);
  return (int32_t)(b[0]) |
         (int32_t)(b[1]) << 8 |
         (int32_t)(b[2]) << 16 |
         (int32_t)(b[3]) << 24;
}

void
ak_write_u32(ak_iostream s, uint32_t n)
{
  uint8_t b[4];
  b[0] = (uint8_t)(n);
  b[1] = (uint8_t)(n >> 8);
  b[2] = (uint8_t)(n >> 16);
  b[3] = (uint8_t)(n >> 24);
  ak_iostream_write(s, b, 4);
}

uint32_t
ak_read_u32(ak_iostream s)
{
  uint8_t b[4];
  int32_t c = ak_iostream_read(s, b, 4);
  ak_assert(c == 4);
  return (uint32_t)(b[0]) |
         (uint32_t)(b[1]) << 8 |
         (uint32_t)(b[2]) << 16 |
         (uint32_t)(b[3]) << 24;
}

void
ak_write_i64(ak_iostream s, int64_t n)
{
  uint8_t b[8];
  b[0] = (uint8_t)(n);
  b[1] = (uint8_t)(n >> 8);
  b[2] = (uint8_t)(n >> 16);
  b[3] = (uint8_t)(n >> 24);
  b[4] = (uint8_t)(n >> 32);
  b[5] = (uint8_t)(n >> 40);
  b[6] = (uint8_t)(n >> 48);
  b[7] = (uint8_t)(n >> 56);
  ak_iostream_write(s, b, 8);
}

int64_t
ak_read_i64(ak_iostream s)
{
  uint8_t b[8];
  int32_t c = ak_iostream_read(s, b, 8);
  ak_assert(c == 8);
  return (int64_t)(b[0]) |
         (int64_t)(b[1]) << 8 |
         (int64_t)(b[2]) << 16 |
         (int64_t)(b[3]) << 24 |
         (int64_t)(b[4]) << 32 |
         (int64_t)(b[5]) << 40 |
         (int64_t)(b[6]) << 48 |
         (int64_t)(b[7]) << 56;
}

void
ak_write_u64(ak_iostream s, uint64_t n)
{
  uint8_t b[8];
  b[0] = (uint8_t)(n);
  b[1] = (uint8_t)(n >> 8);
  b[2] = (uint8_t)(n >> 16);
  b[3] = (uint8_t)(n >> 24);
  b[4] = (uint8_t)(n >> 32);
  b[5] = (uint8_t)(n >> 40);
  b[6] = (uint8_t)(n >> 48);
  b[7] = (uint8_t)(n >> 56);
  ak_iostream_write(s, b, 8);
}

uint64_t
ak_read_u64(ak_iostream s)
{
  uint8_t b[8];
  int32_t c = ak_iostream_read(s, b, 8);
  ak_assert(c == 8);
  return (uint64_t)(b[0]) |
         (uint64_t)(b[1]) << 8 |
         (uint64_t)(b[2]) << 16 |
         (uint64_t)(b[3]) << 24 |
         (uint64_t)(b[4]) << 32 |
         (uint64_t)(b[5]) << 40 |
         (uint64_t)(b[6]) << 48 |
         (uint64_t)(b[7]) << 56;
}

void
ak_write_f(ak_iostream s, float n)
{
  uint32_t tmp;
  memcpy(&tmp, &n, 4);
  ak_write_u32(s, tmp);
}

float
ak_read_f(ak_iostream s)
{
  float n;
  uint32_t tmp = ak_read_u32(s);
  memcpy(&n, &tmp, 4);
  return n;
}

void
ak_write_d(ak_iostream s, double n)
{
  uint64_t tmp;
  memcpy(&tmp, &n, 8);
  ak_write_u64(s, tmp);
}

double
ak_read_d(ak_iostream s)
{
  double n;
  uint64_t tmp = ak_read_u64(s);
  memcpy(&n, &tmp, 8);
  return n;
}
