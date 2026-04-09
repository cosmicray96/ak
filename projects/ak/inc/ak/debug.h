#ifndef ak_debug_h
#define ak_debug_h

#include "ak/core/errcode.h"
#include "ak/core/io.h"
#include "ak/export.h"
#include "ak/program/program.h"

ak_ex void
ak_debug_make(ak_iostream s);

ak_ex void
ak_log_itn(const char* file,
           uint32_t line,
           const char* fmt,
           ...);
ak_ex void
ak_ec_itn(const char* file,
          uint32_t line,
          ak_errcode ec);

ak_ex void
ak_log_crash(const char* file,
             uint32_t line);

ak_ex void
ak_log_cic();

#define ak_logging
#ifdef ak_logging

#define ak_log(fmt, ...)                    \
  do {                                      \
    ak_log_itn(__FILE__,                    \
               __LINE__,                    \
               fmt,                         \
               __VA_ARGS__);                \
  } while (0)

#define ak_ec(ec)                           \
  do {                                      \
    ak_ec_itn(__FILE__, __LINE__, ec);      \
    ak_log_crash(__FILE__, __LINE__);       \
    ak_program_crash();                     \
  } while (0)

#else

#define ak_log(fmt, ...)
#define ak_ec(ec)

#endif

#endif
