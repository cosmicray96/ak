#include "ak/os/file.h"
#include <stdio.h>

//===== ak_file =====//
//--- export ---//
ak_ex ak_errcode
ak_file_open(const char* path,
             ak_file_enum fe,
             ak_file* o_f);

ak_ex void
ak_file_close(ak_file* f);

ak_ex uint32_t
ak_file_size(ak_file* f);

ak_ex uint32_t
ak_file_read(ak_file* f,
             void* o_buf,
             uint32_t count);
ak_ex void
ak_file_write(ak_file* f,
              void* buf,
              uint32_t count);

//===== fstream =====//
//--- private ---//
static int32_t
read(void* ctx,
     void* buff,
     uint32_t read_count)
{
  FILE* f = ctx;
  return fread(buff, 1, read_count, f);
}

static int32_t
write(void* ctx,
      const void* buff,
      uint32_t write_count)
{
  FILE* f = ctx;
  int32_t n =
    fwrite(buff, 1, write_count, f);
  fflush(f);
  return n;
}

static void
close(void* ctx)
{
  FILE* f = ctx;
  fclose(f);
}

//--- export ---//
ak_iostream
ak_fstream_make(const char* path,
                const char* mode)
{
  ak_iostream s = { 0 };
  FILE* f = fopen(path, mode);

  s.ctx = f;
  s.read = &read;
  s.write = &write;
  s.close = &close;
  return s;
}
