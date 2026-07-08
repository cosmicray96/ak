#include "ak/res/reses/aniclip.h"
#include "ak/coll/stm.h"

ak_aniclip
ak_aniclip_make(ak_resid atlasid,
                const ak_da* da,
                ak_alct alct)
{
  ak_aniclip ac = {};
  ac.atlasid = atlasid;
  ac.frames =
    ak_da_make(sizeof(uint32_t), alct);
  uint32_t count = ak_da_count(da);
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t* f =
      ak_da_at_const(da, i);
    ak_da_pushback(&ac.frames, f);
  }
  return ac;
}
ak_errcode
ak_stm_write_aniclip(ak_stm stm,
                     const ak_aniclip* ac)
{
  return ak_stm_write_da(stm, &ac->frames);
}
ak_errcode
ak_stm_read_aniclip(ak_stm stm,
                    ak_resid atlasid,
                    ak_aniclip* o_ac,
                    ak_alct alct)
{
  ak_aniclip ac = { 0 };
  ac.atlasid = atlasid;
  ak_stm_try(
    ak_stm_read_da(stm, &ac.frames, alct));

  *o_ac = ac;
  return ak_ok;
}

void
ak_aniclip_destroy(ak_aniclip* ac)
{
  ak_da_destroy(&ac->frames);
}
