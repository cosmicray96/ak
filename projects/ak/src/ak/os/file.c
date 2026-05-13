#include "ak/os/file.h"
#include "ak/debug.h"

#include <stdio.h>

//===== ak_file =====//
//--- private ---//
const char*
get_mode(ak_file_enum fe)
{
  if (fe == ak_file_open_read)
    return "rb";
  if (fe == ak_file_open_write)
    return "wb";
  if (fe == ak_file_open_append)
    return "wb+";
  return 0;
}

//--- export ---//
ak_file
ak_file_open(const char* path,
             ak_file_enum fe)
{
  ak_file f = { 0 };
  f.fe = fe;
  const char* mode = get_mode(fe);
  ak_assert_ec(mode, ak_err_invalid_args);
  f.impl = (uintptr_t)fopen(path, mode);
  ak_assert_ec(f.impl, ak_err);
  return f;
}

void
ak_file_close(ak_file* f)
{
  fclose((FILE*)f->impl);
}

uint64_t
ak_file_open_read_all(const char* path,
                      void** o_buf,
                      ak_alct alct)
{
  ak_file f =
    ak_file_open(path, ak_file_open_read);
  uint64_t size = ak_file_size(&f);
  *o_buf = ak_alct_alloc(alct, size);
  uint64_t read_size =
    ak_file_read(&f, o_buf, size);
  ak_assert(size == read_size);
  ak_file_close(&f);
  return size;
}

uint64_t
ak_file_size(ak_file* f)
{
  FILE* fp = (FILE*)f->impl;
  long pos = 0;
  long current = 0;
  uint64_t size = 0;
  int err = 0;

  pos = ftell(fp);
  ak_assert(pos >= 0);
  current = pos;

  err = fseek(fp, 0, SEEK_END);
  ak_assert(err == 0);

  pos = ftell(fp);
  ak_assert(pos >= 0);
  size = pos;

  err = fseek(fp, current, SEEK_SET);
  ak_assert(err == 0);

  return size;
}

void
ak_file_seek(ak_file* f, int64_t idx)
{
  FILE* fp = (FILE*)f->impl;
  int err = fseek(fp, (long)idx, SEEK_SET);
  ak_assert(err == 0);
}

uint64_t
ak_file_read(ak_file* f,
             void** o_buf,
             uint64_t count)
{
  FILE* fp = (FILE*)f->impl;
  uint8_t* buf = *o_buf;
  uint64_t read_count = 0;
  uint64_t n = 0;
  while (read_count < count) {
    size_t n =
      fread(buf + read_count,
            1,
            (size_t)(count - read_count),
            fp);

    if (n == 0) {
      break;
    }

    read_count += (uint64_t)n;
  }
  return read_count;
}

void
ak_file_write(ak_file* f,
              void* buf,
              uint64_t count)
{
  ak_assert_ec(f->fe == ak_file_open_write ||
                 f->fe ==
                   ak_file_open_append,
               ak_err_invalid_state);

  FILE* fp = (FILE*)f->impl;
  uint8_t* b = buf;
  uint64_t write_count = 0;
  uint64_t n = 0;
  while (write_count < count) {
    size_t n =
      fwrite(b + write_count,
             1,
             (size_t)(count - write_count),
             fp);

    if (n == 0) {
      break;
    }

    write_count += (uint64_t)n;
  }
}

ak_ex ak_iostream
ak_file_to_iostream(ak_file* f)
{
  ak_iostream io;
  ak_ec(ak_err_not_impled);
  return io;
}

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
