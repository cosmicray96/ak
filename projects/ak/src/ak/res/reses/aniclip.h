#ifndef ak_res_reses_aniclip
#define ak_res_reses_aniclip

#include "ak/coll/da.h"
#include "ak/core/mem/allocator.h"
#include "ak/res/core.h"
#include "ak/system/stream.h"

typedef struct
{
  ak_resid atlasid;
  ak_da frames;
} ak_aniclip;

ak_aniclip
ak_aniclip_make(ak_resid atlasid,
                const ak_da* da,
                ak_alct alct);
ak_stmerr
ak_stm_write_aniclip(ak_stm stm,
                     const ak_aniclip* o_ac);
ak_stmerr
ak_stm_read_aniclip(ak_stm stm,
                    ak_resid atlasid,
                    ak_aniclip* o_ac,
                    ak_alct alct);

void
ak_aniclip_destroy(ak_aniclip* ac);

static uint32_t
ak_aniclip_count(const ak_aniclip* ac)
{
  return ak_da_count(&ac->frames);
}
static uint32_t
ak_aniclip_at(const ak_aniclip* ac,
              uint32_t idx)
{
  return *(uint32_t*)ak_da_at_const(
    &ac->frames, idx);
}

#endif
