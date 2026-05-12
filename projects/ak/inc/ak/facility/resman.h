#ifndef ak_facility_resman_h
#define ak_facility_resman_h

#include "ak/coll/hmn.h"
#include "ak/core/async/jobpool.h"
#include "ak/core/mem/allocator.h"
#include "ak/export.h"
#include "ak/facility/idgen.h"

typedef uint32_t ak_resid;
typedef enum
{
  ak_res_not_loaded,
  ak_res_loading,
  ak_res_loaded
} ak_res_status;

typedef struct
{
  ak_alct alct;
  ak_idgen ig;
  ak_hmn map;
  ak_jobpool* jp;
} ak_resman;

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

#endif
