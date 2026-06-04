#include "ak/res/reg.h"
#include "ak/coll/hmn.h"
#include "ak/core/mem/ptr.h"
#include "ak/debug.h"
#include <stdint.h>

#define ak_s_max_res_size 200
#define ak_s_cutoff ((uint32_t)(1) << 31)

typedef struct
{
  uint8_t data[ak_s_max_res_size];
} item;

//--- export ---//
ak_resreg
ak_resreg_make(ak_alct alct)
{
  ak_resreg rr = { 0 };
  rr.map = ak_hmn_make(sizeof(item), alct);
  rr.counter = ak_s_cutoff;
  return rr;
}

void
ak_resreg_destroy(ak_resreg* rr)
{
  ak_hmn_destroy(&rr->map);
}

void
ak_resreg_reg_w_id(ak_resreg* rr,
                   ak_resid id,
                   const void* res,
                   uint32_t size)
{
  ak_assert(!ak_hmn_exist(&rr->map, id));
  ak_assert(id < ak_s_cutoff);
  ak_assert(size <= ak_s_max_res_size);

  item itm = { 0 };
  ak_p_cpy(itm.data, res, size);

  ak_hmn_insert(&rr->map, id, &itm);
}

ak_resid
ak_resreg_reg(ak_resreg* rr,
              const void* res,
              uint32_t size)
{
  ak_assert(size <= ak_s_max_res_size);

  ak_resid id = rr->counter;
  rr->counter++;

  item itm = { 0 };
  ak_p_cpy(itm.data, res, size);

  ak_hmn_insert(&rr->map, id, &itm);
  return id;
}

void
ak_resreg_unreg(ak_resreg* rr, ak_resid id)
{
  ak_hmn_remove(&rr->map, id);
}

void*
ak_resreg_get(ak_resreg* rr, ak_resid id)
{
  ak_assert(ak_hmn_exist(&rr->map, id));

  item* itm = ak_hmn_at(&rr->map, id);
  return itm->data;
}
