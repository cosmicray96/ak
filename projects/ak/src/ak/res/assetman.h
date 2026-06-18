#ifndef ak_res_assetman_h
#define ak_res_assetman_h

#include "ak/coll/hmn.h"
#include "ak/core/async/thpool.h"
#include "ak/core/mem/allocator.h"
#include "ak/gfx/core.h"
#include "ak/gfx/gresman.h"
#include "ak/gfx/gresreg.h"
#include "ak/gfx/tex.h"
#include "ak/res/core.h"
#include "ak/res/reg.h"
#include "ak/res/resman.h"

typedef struct
{
  ak_restype type;
  const char* path;
  union
  {
    struct
    {
      ak_gresid tex_gid;
    } texatlas;
    struct
    {
      ak_resid atlas_rid;
    } aniclip;
  };
} ak_assetman_rargs;

typedef struct
{
  ak_grestype type;
  ak_resid rid;
  union
  {
    struct
    {
      ak_textype textype;
    } tex;
  };
} ak_assetman_gargs;

typedef struct
{
  ak_thpool* tp;
  ak_resman* rm;
  ak_gresman* grm;
  ak_hmn res_map;
  ak_hmn gres_map;
} ak_assetman;

ak_assetman
ak_assetman_make(ak_thpool* tp,
                 ak_resman* rm,
                 ak_gresman* grm,
                 ak_alct alct);
void
ak_assetman_destroy(ak_assetman* am);

void
ak_assetman_load_res(ak_assetman* am,
                     ak_resid rid);
void
ak_assetman_unload_res(ak_assetman* am,
                       ak_resid rid);

void
ak_assetman_load_gres(ak_assetman* am,
                      ak_gresid gid);
void
ak_assetman_unload_gres(ak_assetman* am,
                        ak_gresid gid);

void
ak_assetman_reg_res(
  ak_assetman* am,
  ak_resid rid,
  const ak_assetman_rargs* rargs);

void
ak_assetman_reg_gres(
  ak_assetman* am,
  ak_gresid gid,
  const ak_assetman_gargs* gargs);

#endif
