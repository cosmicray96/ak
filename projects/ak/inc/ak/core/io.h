#ifndef ak_io_h
#define ak_io_h

#include "ak/export.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct
{
  int32_t (*read)(void* ctx,
                  void* buff,
                  uint32_t read_count);

  int32_t (*write)(void* ctx,
                   const void* buff,
                   uint32_t write_count);

  void (*close)(void* ctx);

  void* ctx;
} ak_iostream;

static int32_t
ak_iostream_read(ak_iostream s,
                 void* buff,
                 uint32_t read_count)
{
  return s.read(s.ctx, buff, read_count);
}

static int32_t
ak_iostream_write(ak_iostream s,
                  const void* buff,
                  uint32_t write_count)
{
  return s.write(s.ctx, buff, write_count);
}

static void
ak_iostream_close(ak_iostream* s)
{
  s->close(s->ctx);
  *s = (ak_iostream){ 0 };
}

ak_ex void
ak_iostream_write_str(ak_iostream s,
                      const char* str);

ak_ex void
ak_iostream_print_u64(ak_iostream io,
                      uint64_t n);
ak_ex void
ak_iostream_print_d(ak_iostream io,
                    double d);

ak_ex ak_iostream
ak_iostream_sio();

ak_ex void
ak_write_bool(ak_iostream s, bool n);
ak_ex bool
ak_read_bool(ak_iostream s);

ak_ex void
ak_write_i16(ak_iostream s, int16_t n);
ak_ex int16_t
ak_read_i16(ak_iostream s);

ak_ex void
ak_write_u16(ak_iostream s, uint16_t n);
ak_ex uint16_t
ak_read_u16(ak_iostream s);

ak_ex void
ak_write_i32(ak_iostream s, int32_t n);
ak_ex int32_t
ak_read_i32(ak_iostream s);

ak_ex void
ak_write_u32(ak_iostream s, uint32_t n);
ak_ex uint32_t
ak_read_u32(ak_iostream s);

ak_ex void
ak_write_i64(ak_iostream s, int64_t n);
ak_ex int64_t
ak_read_i64(ak_iostream s);

ak_ex void
ak_write_u64(ak_iostream s, uint64_t n);
ak_ex uint64_t
ak_read_u64(ak_iostream s);

ak_ex void
ak_write_f(ak_iostream s, float n);
ak_ex float
ak_read_f(ak_iostream s);

ak_ex void
ak_write_d(ak_iostream s, double n);
ak_ex double
ak_read_d(ak_iostream s);

#endif
