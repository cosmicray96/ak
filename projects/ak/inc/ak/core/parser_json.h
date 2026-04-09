#ifndef ak_core_parser_json_h
#define ak_core_parser_json_h

#include "ak/core/io.h"
#include "ak/core/mem/allocator.h"
#include "ak/export.h"
#include <stdint.h>

typedef struct
{
  //...
} ak_json_w;

ak_ex ak_json_w
ak_json_w_make(ak_iostream s, ak_alct alct);
ak_ex void
ak_json_w_destroy(ak_json_w* w);

ak_ex void
ak_json_w_int_pair(ak_json_w* w,
                   const char* name,
                   int64_t n);
ak_ex void
ak_json_w_float_pair(ak_json_w* w,
                     const char* name,
                     double n);
ak_ex void
ak_json_w_str_pair(ak_json_w* w,
                   const char* name,
                   const char* str);

ak_ex void
ak_json_w_int_val(ak_json_w* w, int64_t n);
ak_ex void
ak_json_w_float_val(ak_json_w* w, double n);
ak_ex void
ak_json_w_str_val(ak_json_w* w,
                  const char* str);

ak_ex void
ak_json_w_obj_begin(ak_json_w* w,
                    const char* name);
ak_ex void
ak_json_w_obj_end(ak_json_w* w);
ak_ex void
ak_json_w_arr_begin(ak_json_w* w,
                    const char* name);
ak_ex void
ak_json_w_arr_end(ak_json_w* w);

typedef struct ak_json_obj ak_json_obj;

#endif
