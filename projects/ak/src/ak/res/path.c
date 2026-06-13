#include "ak/res/path.h"
#include "ak/gfx/gfx.h"
#include "ak/res/reg.h"
#include <stdio.h>

//===== ak_respathman =====//
//--- private ---//
#define s_path_size 512
#define s_base_dir "./assets"

typedef struct
{
  ak_restype type;
  char path[s_path_size];
} item;

//--- internal ---//
ak_respathman
ak_respathman_make(ak_alct alct)
{
  ak_respathman rpm = { 0 };
  rpm.map = ak_hmn_make(sizeof(item), alct);
  return rpm;
}

void
ak_respathman_destroy(ak_respathman* rpm)
{
  ak_hmn_destroy(&rpm->map);
}

void
ak_respathman_reg(ak_respathman* rpm,
                  ak_resid id,
                  ak_restype type,
                  const char* path)
{
  item itm = { .type = type };
  int n = 0;

  switch (type) {
    case ak_restype_shaderstr: {
      n = snprintf(itm.path,
                   s_path_size,
                   "%s/%s",
                   ak_gfx_name(),
                   s_base_dir);

      break;
    }
    default: {
      n = snprintf(itm.path,
                   s_path_size,
                   "%s/%s",
                   "common",
                   s_base_dir);
      break;
    }
  }

  snprintf(itm.path + n,
           s_path_size - n,
           "%s/%s",
           s_base_dir,
           path);
}

const char*
ak_respathman_get(ak_respathman* rpm,
                  ak_resid id)
{
  item* itm = ak_hmn_at(&rpm->map, id);
  return itm->path;
}
