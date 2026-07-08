#ifndef ak_core_math_stm_h
#define ak_core_math_stm_h

#include "ak/core/math/mat3x3.h"
#include "ak/core/math/trig.h"
#include "ak/core/math/vec2f.h"
#include "ak/core/math/vec3.h"
#include "ak/core/math/vec3f.h"
#include "ak/core/math/vec4.h"
#include "ak/core/math/vec4f.h"
#include "ak/export.h"
#include "ak/system/stream.h"

ak_ex ak_errcode
ak_stm_write_angle(ak_stm stm,
                   ak_angle angle);
ak_ex ak_errcode
ak_stm_read_angle(ak_stm stm,
                  ak_angle* o_angle);

ak_ex ak_errcode
ak_stm_write_vec2(ak_stm stm, ak_vec2 vec2);
ak_ex ak_errcode
ak_stm_read_vec2(ak_stm stm,
                 ak_vec2* o_vec2);

ak_ex ak_errcode
ak_stm_write_vec3(ak_stm stm, ak_vec3 vec3);
ak_ex ak_errcode
ak_stm_read_vec3(ak_stm stm,
                 ak_vec3* o_vec3);

ak_ex ak_errcode
ak_stm_write_vec4(ak_stm stm, ak_vec4 vec4);
ak_ex ak_errcode
ak_stm_read_vec4(ak_stm stm,
                 ak_vec4* o_vec4);

ak_ex ak_errcode
ak_stm_write_vec2f(ak_stm stm,
                   ak_vec2f vec2f);
ak_ex ak_errcode
ak_stm_read_vec2f(ak_stm stm,
                  ak_vec2f* o_vec2f);

ak_ex ak_errcode
ak_stm_write_vec3f(ak_stm stm,
                   ak_vec3f vec3f);
ak_ex ak_errcode
ak_stm_read_vec3f(ak_stm stm,
                  ak_vec3f* o_vec3f);

ak_ex ak_errcode
ak_stm_write_vec4f(ak_stm stm,
                   ak_vec4f vec4f);

ak_ex ak_errcode
ak_stm_read_vec4f(ak_stm stm,
                  ak_vec4f* o_vec4f);

ak_ex ak_errcode
ak_stm_write_mat3(ak_stm stm, ak_mat3 mat3);
ak_ex ak_errcode
ak_stm_read_mat3(ak_stm stm,
                 ak_mat3* o_mat3);

#endif
