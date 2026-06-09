#include "ak/res/assetman.h"
#include "ak/gfx/gresreg.h"
#include "ak/gfx/tex.h"
#include "ak/res/reg.h"

typedef struct
{
  ak_gresid gid;
  ak_grestype type;
  union
  {
    struct
    {
      ak_resid img_rid;
    } tex;
  };
} loading_item;

typedef struct
{
  ak_resid rid;
  ak_restype type;
  const char* path;
} res_item;

typedef struct
{
  ak_gresid gid;
  ak_grestype type;
  union
  {
    struct
    {
      ak_resid img_rid;
      ak_textype type;
    } tex;
  };
} gres_item;

ak_assetman
ak_assetman_make(ak_resman* rm,
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
ak_assetman_reg_res_img(ak_assetman* am,
                        ak_resid rid,
                        const char* path);
void
ak_assetman_reg_res_world(ak_assetman* am,
                          ak_resid rid,
                          const char* path);

void
ak_assetman_reg_gres_tex(ak_assetman* am,
                         ak_gresid gid,
                         ak_resid rid,
                         ak_textype type);
