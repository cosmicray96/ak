#ifndef ak_res_astload_h
#define ak_res_astload_h

#include "ak/coll/da.h"
#include "ak/core/async/thpool.h"
#include "ak/core/mem/allocator.h"
#include "ak/gfx/core.h"
#include "ak/gfx/gresman.h"
#include "ak/gfx/tex.h"
#include "ak/res/reg.h"
#include "ak/res/resman.h"
#include "ak/system/stream.h"

void
ak_astload_gres_load_tex(ak_thpool* th,
                         ak_resman* rm,
                         ak_gresman* grm,
                         ak_gresid gid,
                         ak_textype textype,
                         ak_resid rid,
                         ak_stm stm);

void
ak_astload_gres_unload_tex(ak_thpool* th,
                           ak_gresman* grm,
                           ak_gresid gid);

void
ak_astload_gres_load_shader(ak_thpool* th,
                            ak_resman* rm,
                            ak_gresman* grm,
                            ak_gresid gid,
                            ak_resid rid,
                            ak_stm stm);

void
ak_astload_gres_unload_shader(
  ak_thpool* th,
  ak_gresman* grm,
  ak_gresid gid);

#endif
