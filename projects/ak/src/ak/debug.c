#include "ak/debug.h"

void
ak_debug_make(ak_iostream s)
{
}

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
