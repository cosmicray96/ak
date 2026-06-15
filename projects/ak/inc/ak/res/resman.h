#ifndef ak_system_resman_h
#define ak_system_resman_h

#include "ak/export.h"
#include "ak/res/reg.h"
#include "ak/system/stream.h"

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
  ak_res_not_exist,
  ak_res_not_loaded,
  ak_res_loading,
  ak_res_loaded
} ak_res_status;

typedef struct ak_resman ak_resman;

ak_ex ak_res_status
ak_resman_status(ak_resman* rm, ak_resid id);

ak_ex void
ak_resman_load(ak_resman* rm,
               ak_resid id,
               ak_restype type,
               ak_stm stm,
               bool stm_close);
ak_ex void
ak_resman_unload(ak_resman* rm, ak_resid id);

#endif
