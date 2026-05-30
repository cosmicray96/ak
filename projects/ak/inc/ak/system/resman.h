#ifndef ak_system_resman_h
#define ak_system_resman_h

#include "ak/export.h"
#include "ak/game/stg/world.h"
#include "ak/system/idgen.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct
{
  void* data;
  uint64_t size;
} ak_res_file;

typedef enum
{
  ak_img_rgba8
} ak_imgtype;

typedef struct
{
  ak_imgtype type;
  void* pixels;
  uint32_t w;
  uint32_t h;
} ak_res_img;
typedef ak_res_img ak_img;

typedef struct ak_resman ak_resman;

typedef uint32_t ak_resid;
typedef enum
{
  ak_restype_none = 0,
  ak_restype_file,
  ak_restype_img,
  ak_restype_world,
} ak_restype;

typedef enum
{
  ak_res_not_loaded,
  ak_res_loading,
  ak_res_loaded
} ak_res_status;

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
ak_resman_release(ak_resman* rm,
                  ak_resid id);

ak_ex void
ak_resman_register_file(ak_resman* rm,
                        ak_resid id,
                        const char* path);
ak_ex bool
ak_resman_acquire_file(ak_resman* rm,
                       ak_resid id,
                       ak_res_file* o_file);

ak_ex void
ak_resman_register_img(ak_resman* rm,
                       ak_resid id,
                       const char* path);
ak_ex bool
ak_resman_acquire_img(ak_resman* rm,
                      ak_resid id,
                      ak_res_img* o_img);

ak_ex void
ak_resman_register_world(ak_resman* rm,
                         ak_resid id,
                         const char* path,
                         ak_idgen* ig);
ak_ex bool
ak_resman_acquire_world(ak_resman* rm,
                        ak_resid id,
                        ak_world* o_world,
                        ak_stmerr* o_err);

#endif
