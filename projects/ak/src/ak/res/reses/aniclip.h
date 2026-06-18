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
ak_stm_read_aniclip(ak_stm stm,
                    ak_resid atlasid,
                    ak_aniclip* o_ac,
                    ak_alct alct);

void
ak_aniclip_destroy(ak_aniclip* ac);

#endif
