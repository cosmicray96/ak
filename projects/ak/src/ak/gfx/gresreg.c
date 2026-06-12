#include "ak/gfx/gresreg.h"
#include "ak/coll/hmn.h"
#include "ak/core/mem/allocator.h"
#include "ak/debug.h"
#include "ak/gfx/core.h"
#include "ak/gfx/shader.h"
#include "ak/gfx/tex.h"

//--- private ---//
typedef struct
{
  ak_grestype type;
  void* gres;
} item;

//===== ak_gresreg =====//
//--- private ---//
struct ak_gresreg
{
  ak_alct alct;
  ak_gfx* gfx;
  ak_hmn map;
};

static void*
gres_get(ak_gresreg* grr, ak_gresid id)
{
  item* itm = ak_hmn_at(&grr->map, id);
  return itm->gres;
}
static void
gres_unreg(ak_gresreg* grr, ak_gresid id)
{
  item* itm = ak_hmn_at(&grr->map, id);
  switch (itm->type) {
    case ak_grestype_tex: {
      ak_tex_destroy(itm->gres);
      break;
    }
    case ak_grestype_shader: {
      ak_shader_destroy(itm->gres);
      break;
    }

    default: {
      ak_assert(false);
    }
  }
}

//--- internal ---//
ak_gresreg*
ak_gresreg_make(ak_gfx* gfx, ak_alct alct)
{

  ak_gresreg* grr =
    ak_alct_alloc(alct, sizeof(ak_gresreg));
  grr->alct = alct;
  grr->gfx = gfx;
  grr->map = ak_hmn_make(sizeof(item), alct);
  return grr;
}

void
ak_gresreg_destroy(ak_gresreg* grr)
{

  ak_hmn_iter it =
    ak_hmn_iter_make(&grr->map);
  uint64_t key = 0;
  void* value = 0;
  while (
    ak_hmn_iter_next(&it, &key, &value)) {
    ak_gresid id = key;
    gres_unreg(grr, id);
  }

  ak_hmn_destroy(&grr->map);
  ak_alct_free(grr->alct, grr);
}

void
ak_gresreg_reg(ak_gresreg* grr,
               ak_gresid id,
               ak_grestype type,
               void* gres)
{
  item itm = { .type = type, .gres = gres };
  ak_hmn_insert(&grr->map, id, &itm);
}

void
ak_gresreg_unreg(ak_gresreg* grr,
                 ak_gresid id)
{

  gres_unreg(grr, id);
  ak_hmn_remove(&grr->map, id);
}

ak_tex*
ak_gresreg_get_tex(ak_gresreg* grr,
                   ak_gresid id)
{
  return gres_get(grr, id);
}

ak_shader*
ak_gresreg_get_shader(ak_gresreg* grr,
                      ak_gresid id)
{
  return gres_get(grr, id);
}
