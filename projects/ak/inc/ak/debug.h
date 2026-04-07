#ifndef ak_debug_h
#define ak_debug_h

#include "ak/core/errcode.h"
#include "ak/core/io.h"
#include "ak/export.h"
#include "ak/program/program.h"

ak_ex void
ak_debug_make(ak_iostream s);

ak_ex void
ak_debug_log_itn(const char* file,
                 uint32_t line,
                 const char* fmt,
                 ...);
ak_ex void
ak_debug_log_ec_itn(const char* file,
                    uint32_t line,
                    ak_errcode ec);

ak_ex void
ak_debig_log_exit_itn(const char* file,
                      uint32_t line);

#define ak_logging
#ifdef ak_logging

#define ak_log(fmt, ...)                    \
  do {                                      \
    ak_debug_log_itn(__FILE__,              \
                     __LINE__,              \
                     fmt,                   \
                     __VA_ARGS__);          \
  } while (0)

#define ak_log_ec(ec)                       \
  do {                                      \
    ak_debug_log_ec_itn(                    \
      __FILE__, __LINE__, ec);              \
    ak_debug_log_exit_itn(__FILE__,         \
                          __LINE__);        \
    ak_program_exit();                      \
  } while (0)

#else

#define ak_log(fmt, ...)
#define ak_log_ec(ec)

#endif

#endif
