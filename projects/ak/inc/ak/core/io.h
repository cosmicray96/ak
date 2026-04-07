#ifndef ak_io_h
#define ak_io_h

#include <stdint.h>

typedef struct
{
  int32_t (*read)(void* ctx,
                  void* buff,
                  uint32_t read_count);

  int32_t (*write)(void* ctx,
                   void* buff,
                   uint32_t write_count);

  void (*close)(void* ctx);

  void* ctx;
} ak_iostream;

static int32_t
ak_iostream_read(ak_iostream* s,
                 void* buff,
                 uint32_t read_count)
{
  return s->read(s->ctx, buff, read_count);
}

static int32_t
ak_iostream_write(ak_iostream* s,
                  void* buff,
                  uint32_t write_count)
{
  return s->write(s->ctx, buff, write_count);
}

static void
ak_iostream_close(ak_iostream* s)
{
  s->close(s->ctx);
}

#endif
