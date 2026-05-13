#ifndef ak_os_file_h
#define ak_os_file_h

#include "ak/core/errcode.h"
#include "ak/core/io.h"
#include "ak/export.h"
#include <stdint.h>

typedef enum
{
  ak_file_none = 0,
  ak_file_open_read,
  ak_file_open_write
} ak_file_enum;
typedef struct
{
  uintptr_t impl;
  ak_file_enum fe;
} ak_file;

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

ak_ex ak_iostream
ak_fstream_make(const char* path,
                const char* mode);

ak_ex ak_iostream
ak_frawstream_make(const char* path,
                   const char* mode);

#endif
