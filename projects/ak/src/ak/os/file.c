#include "ak/os/file.h"
#include <stdio.h>

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
  return fwrite(buff, 1, write_count, f);
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
