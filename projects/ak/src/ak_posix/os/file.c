#include "ak/os/file.h"
#include "ak/debug.h"

#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

//--- private ---//
int
fopen_mode_to_flags(const char* mode)
{
  if (strcmp(mode, "r") == 0)
    return O_RDONLY;
  if (strcmp(mode, "r+") == 0)
    return O_RDWR;
  if (strcmp(mode, "w") == 0)
    return O_WRONLY | O_CREAT | O_TRUNC;
  if (strcmp(mode, "w+") == 0)
    return O_RDWR | O_CREAT | O_TRUNC;
  if (strcmp(mode, "a") == 0)
    return O_WRONLY | O_CREAT | O_APPEND;
  if (strcmp(mode, "a+") == 0)
    return O_RDWR | O_CREAT | O_APPEND;
  return -1; // unknown mode
}

//===== fstream =====//
//--- private ---//
static int32_t
file_read(void* ctx,
          void* buff,
          uint32_t read_count)
{
  int f = (int)(uintptr_t)ctx;
  return read(f, buff, read_count);
}

static int32_t
file_write(void* ctx,
           const void* buff,
           uint32_t write_count)
{
  int f = (int)(uintptr_t)ctx;
  return write(f, buff, write_count);
}

static void
file_close(void* ctx)
{
  int f = (int)(uintptr_t)ctx;
  close(f);
}

//--- export ---//
ak_iostream
ak_frawstream_make(const char* path,
                   const char* mode)
{
  ak_iostream s = { 0 };
  int flags = fopen_mode_to_flags(mode);
  ak_assert(flags >= 0);

  int f = open(path, flags, 0644);
  ak_assert(f > 0);

  s.ctx = (void*)(uintptr_t)f;
  s.read = &file_read;
  s.write = &file_write;
  s.close = &file_close;
  return s;
}
