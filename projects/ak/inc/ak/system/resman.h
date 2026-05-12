#ifndef ak_system_resman_h
#define ak_system_resman_h

#include "ak/export.h"

#include <stdint.h>

typedef struct ak_resman ak_resman;

typedef uint32_t ak_resid;
typedef enum
{
  ak_res_not_loaded,
  ak_res_loading,
  ak_res_loaded
} ak_res_status;

ak_ex ak_resid
ak_resman_register_file(ak_resman* rm,
                        const char* path);

ak_ex void
ak_resman_load(ak_resman* rm, ak_resid id);
ak_ex void
ak_resman_unload(ak_resman* rm, ak_resid id);
ak_ex ak_res_status
ak_resman_status(ak_resman* rm, ak_resid id);
ak_ex void*
ak_resman_at(ak_resman* rm, ak_resid id);

#endif
