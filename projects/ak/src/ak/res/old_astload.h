/*
#ifndef ak_res_astload_h
#define ak_res_astload_h

#include "ak/core/async/thpool.h"
#include "ak/gfx/gresman.h"
#include "ak/gfx/tex.h"
#include "ak/res/resman.h"

void
ak_astload_res_unload(ak_thpool* tp,
                      ak_resman* rm,
                      ak_resid rid);
void
ak_astload_gres_unload(ak_thpool* tp,
                       ak_gresman* grm,
                       ak_gresid gid);

void
ak_astload_res_load_texatlas(
  ak_thpool* th,
  ak_resman* rm,
  ak_gresman* grm,
  ak_resid ra_rid,
  ak_stm stm,
  ak_gresid tex_gid);

void
ak_astload_res_load_aniclip(ak_thpool* th,
                            ak_resman* rm,
                            ak_resid ac_rid,
                            ak_stm stm,
                            ak_resid ta_rid);

void
ak_astload_gres_load_tex(ak_thpool* th,
                         ak_resman* rm,
                         ak_gresman* grm,
                         ak_gresid tex_gid,
                         ak_textype textype,
                         ak_resid img_rid);

void
ak_astload_gres_load_shader(ak_thpool* th,
                            ak_resman* rm,
                            ak_gresman* grm,
                            ak_gresid s_gid,
                            ak_resid ss_rid);

#endif
*/
