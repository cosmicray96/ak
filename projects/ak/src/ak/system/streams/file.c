#include "ak/core/errcode.h"
#include "ak/core/mem/allocator.h"
#include "ak/system/stream_itn.h"

#include <errno.h>
#include <stdio.h>

ak_errcode
ak_stm_open_file(const char* path,
                 const char* mode,
                 ak_stm* o_stm)
{
  FILE* f = fopen(path, mode);
  if (!f) {
    if (errno == ENOENT)
      return ak_err_not_found;
    return ak_err;
  }
  o_stm->type = ak_stmtype_file;
  o_stm->ctx = f;
  return ak_ok;
}

ak_errcode
ak_stm_file_close(void* file)
{
  FILE* f = file;
  fclose(f);
  return ak_ok;
}

ak_stmresult
ak_stm_file_write(void* file,
                  const void* data,
                  uint64_t size)
{
  size_t n =
    fwrite(data, 1, size, (FILE*)file);
  ak_stmresult r = { .transferred = n };
  if (n == size) {
    r.err = ak_ok;
  } else {
    r.err = ak_err;
  }
  return r;
}

ak_stmresult
ak_stm_file_read(void* file,
                 void* data,
                 uint64_t size)
{
  FILE* f = file;
  size_t n = fread(data, 1, size, f);
  ak_stmresult r = { .transferred = n };
  if (n == size) {
    r.err = ak_ok;
  } else if (feof(f)) {
    r.err = ak_err_stm_end;
  } else {
    r.err = ak_err;
  }
  return r;
}

ak_stmresult
ak_stm_file_read_all(void* file,
                     void** o_data,
                     uint64_t* o_size,
                     ak_alct alct)
{
  FILE* f = file;
  ak_stmresult r = { 0 };

  if (fseek(f, 0, SEEK_END) != 0) {
    r.err = ak_err_invalid_operation;
    return r;
  }

  long size = ftell(f);
  if (size < 0) {
    r.err = ak_err;
    return r;
  }
  rewind(f);

  uint8_t* buf =
    ak_alct_alloc(alct, (uint64_t)size);

  size_t n = fread(buf, 1, (size_t)size, f);
  r.transferred = n;

  if (n == (size_t)size) {
    r.err = ak_ok;
    *o_data = buf;
    *o_size = (uint64_t)size;
  } else {
    r.err = ak_err;
    ak_alct_free(alct, buf);
  }
  return r;
}
