#ifndef ak_os_file_h
#define ak_os_file_h

#include "ak/core/io.h"
#include "ak/export.h"

ak_ex ak_iostream
ak_fstream_make(const char* path,
                const char* mode);

ak_ex ak_iostream
ak_frawstream_make(const char* path,
                   const char* mode);

#endif
