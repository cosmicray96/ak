#ifndef ak_system_resman_h
#define ak_system_resman_h

#include "ak/core/img.h"
#include "ak/export.h"
#include "ak/game/stg/world.h"
#include "ak/res/reg.h"

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
  ak_res_not_loaded,
  ak_res_loading,
  ak_res_loaded
} ak_res_status;

typedef struct ak_resman ak_resman;

ak_ex ak_res_status
ak_resman_status(ak_resman* rm, ak_resid id);
ak_ex ak_restype
ak_resman_res_type(ak_resman* rm,
                   ak_resid rid);

ak_ex void
ak_resman_load(ak_resman* rm, ak_resid id);
ak_ex void
ak_resman_unload(ak_resman* rm, ak_resid id);

ak_ex void
ak_resman_register_img(ak_resman* rm,
                       ak_resid id,
                       const char* path);

ak_ex void
ak_resman_register_world(ak_resman* rm,
                         ak_resid id,
                         const char* path);

#endif
