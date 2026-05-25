#include "ak/system/stream_itn.h"
#include <stdio.h>

ak_stm
ak_stm_open_file(const char* path,
                 const char* mode)
{
  ak_stm s = { 0 };
  s.type = ak_stmtype_file;
  s.ctx = fopen(path, mode);
  return s;
}

ak_stmerr
ak_stm_file_close(void* file)
{
  FILE* f = file;
  fclose(f);
  return ak_stmerr_ok;
}

ak_stmerr
ak_stm_file_write(void* file,
                  const void* data,
                  uint64_t size)
{
  fwrite(data, 1, size, (FILE*)file);
  return ak_stmerr_ok;
}

ak_ex ak_stmerr
ak_stm_file_read(void* file,
                 void* data,
                 uint64_t size)
{
  fread(data, 1, size, (FILE*)file);
  return ak_stmerr_ok;
}
