/*
#include "ak/coll/hmn.h"
#include "ak/core/mem/allocator.h"
#include "ak/debug.h"
#include "ak/game/stg/world.h"
#include "ak/res/reg.h"
#include "ak/res/reses/aniclip.h"
#include "ak/res/reses/img.h"
#include "ak/res/reses/texatlas.h"
#include <stdint.h>

//--- private ---//
typedef struct
{
  ak_restype type;
  uint32_t idx;
} item;

//===== ak_resreg =====//
//--- private ---//
struct ak_resreg
{
  ak_alct alct;
  ak_sla ress[ak_restype_count];
  ak_hmn map;
};

static void
res_destroy(ak_resreg* rr, ak_resid id)
{
  item* itm = ak_hmn_at(&rr->map, id);
  ak_sla* sla = &rr->ress[itm->type];
  void* p = ak_sla_at(sla, itm->idx);

  switch (itm->type) {
    case ak_restype_image: {
      ak_img* img = p;
      ak_img_destroy(img);
      break;
    }
    case ak_restype_world: {
      ak_world* world = p;
      ak_world_destroy(world);
      break;
    }
    case ak_restype_shaderstr: {
      ak_shaderstr* ss = p;
      ak_shaderstr_destroy(ss);
      break;
    }
    case ak_restype_texatlas: {
      ak_texatlas* ta = p;
      ak_texatlas_destroy(ta);
      break;
    }
    case ak_restype_aniclip: {
      ak_aniclip* ac = p;
      ak_aniclip_destroy(ac);
      break;
    }
    default: {
      ak_assert(false);
    }
  }
  ak_sla_remove(sla, itm->idx);
}

//--- export ---//
ak_resreg*
ak_resreg_make(ak_alct alct)
{
  ak_resreg* rr =
    ak_alct_alloc(alct, sizeof(ak_resreg));
  rr->alct = alct;

  rr->map = ak_hmn_make(sizeof(item), alct);

  rr->ress[ak_restype_image] =
    ak_sla_make(sizeof(ak_img), alct);

  rr->ress[ak_restype_world] =
    ak_sla_make(sizeof(ak_world), alct);

  rr->ress[ak_restype_shaderstr] =
    ak_sla_make(sizeof(ak_shaderstr), alct);

  rr->ress[ak_restype_texatlas] =
    ak_sla_make(sizeof(ak_texatlas), alct);

  rr->ress[ak_restype_aniclip] =
    ak_sla_make(sizeof(ak_aniclip), alct);

  return rr;
}

void
ak_resreg_destroy(ak_resreg* rr)
{

  ak_hmn_iter it =
    ak_hmn_iter_make(&rr->map);
  uint64_t key = 0;
  void* value = 0;
  while (
    ak_hmn_iter_next(&it, &key, &value)) {
    ak_resid id = key;
    res_destroy(rr, id);
  }

  for (uint32_t i = 0; i < ak_restype_count;
       i++) {
    ak_sla_destroy(&rr->ress[i]);
  }

  ak_hmn_destroy(&rr->map);
  ak_alct_free(rr->alct, rr);
}

void
ak_resreg_reg(ak_resreg* rr,
              ak_resid id,
              ak_restype type,
              const void* res)
{
  uint32_t idx =
    ak_sla_insert(&rr->ress[type], res);

  item itm = { .type = type, .idx = idx };
  ak_hmn_insert(&rr->map, id, &itm);
}

void
ak_resreg_unreg(ak_resreg* rr, ak_resid id)
{
  res_destroy(rr, id);
  ak_hmn_remove(&rr->map, id);
}

ak_img
ak_resreg_get_image(ak_resreg* rr,
                    ak_resid id)
{
  item* itm = ak_hmn_at(&rr->map, id);
  ak_sla* sla = &rr->ress[itm->type];
  ak_img* img = ak_sla_at(sla, itm->idx);
  return *img;
}

ak_world
ak_resreg_get_world(ak_resreg* rr,
                    ak_resid id)
{
  item* itm = ak_hmn_at(&rr->map, id);
  ak_assert(itm->type == ak_restype_world);
  ak_sla* sla = &rr->ress[itm->type];
  ak_world* w = ak_sla_at(sla, itm->idx);
  return *w;
}

ak_shaderstr
ak_resreg_get_shaderstr(ak_resreg* rr,
                        ak_resid id)
{
  item* itm = ak_hmn_at(&rr->map, id);
  ak_assert(itm->type ==
            ak_restype_shaderstr);
  ak_sla* sla = &rr->ress[itm->type];
  ak_shaderstr* ss =
    ak_sla_at(sla, itm->idx);
  return *ss;
}

ak_texatlas
ak_resreg_get_texatlas(ak_resreg* rr,
                       ak_resid id)
{
  item* itm = ak_hmn_at(&rr->map, id);
  ak_assert(itm->type ==
            ak_restype_texatlas);
  ak_sla* sla = &rr->ress[itm->type];
  ak_texatlas* ta = ak_sla_at(sla, itm->idx);
  return *ta;
}

ak_aniclip
ak_resreg_get_aniclip(ak_resreg* rr,
                      ak_resid id)
{
  item* itm = ak_hmn_at(&rr->map, id);
  ak_assert(itm->type == ak_restype_aniclip);
  ak_sla* sla = &rr->ress[itm->type];
  ak_aniclip* ac = ak_sla_at(sla, itm->idx);
  return *ac;
}
*/
