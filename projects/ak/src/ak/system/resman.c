#include "ak/system/resman.h"
#include "ak/system/resman_itn.h"

ak_ex ak_resman
ak_resman_make(ak_jobpool* jp, ak_alct alct);
ak_ex void
ak_resman_destroy(ak_resman* rm);

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
