#include "ak/core/mem/allocator.h"
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
  size_t n =
    fread(data, 1, size, (FILE*)file);
  if (n < size) {
    if (feof((FILE*)file))
      return ak_stmerr_end;
    if (ferror((FILE*)file))
      return ak_stmerr_err;
  }
  return ak_stmerr_ok;
}

ak_stmerr
ak_stm_file_read_all(void* file,
                     void** o_data,
                     uint64_t* o_size,
                     ak_alct alct)
{
  FILE* f = file;

  fseek(f, 0, SEEK_END);
  long size = ftell(f);
  rewind(f);

  uint8_t* buf = ak_alct_alloc(alct, size);

  fread(buf, 1, size, f);

  *o_data = buf;
  *o_size = size;
  return ak_stmerr_ok;
}
