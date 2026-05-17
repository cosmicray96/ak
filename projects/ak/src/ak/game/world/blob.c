#include "ak/game/world/blob.h"
#include "ak/coll/da.h"
#include "ak/core/mem/allocator.h"
#include "ak/game/comp.h"
#include "ak/game/stg/core.h"
#include "ak/game/stg/world.h"
#include "ak/game/world/view.h"
#include "ak/game/world/view_itn.h"
#include <stdint.h>

ak_world_blob
ak_world_blob_make(ak_world* w,
                   ak_ett root,
                   ak_alct alct)
{

  uint32_t ett_count = ak_world_ett_count(w);

  ak_da ett_blobs =
    ak_da_make(sizeof(ak_ett_blob), alct);
  ak_da_reserve(&ett_blobs, ett_count);

  ak_da comp_blobs =
    ak_da_make(sizeof(ak_comp_tu), alct);
  ak_da_reserve(&comp_blobs, ett_count * 3);
  uint32_t ett_idx = 0;

  ak_wv wv = ak_wv_make(w);

  ak_wv_itbfs it =
    ak_wv_itbfs_make(&wv, root, alct);

  uint32_t i = 0;
  ak_ett e = 0;
  while (ak_wv_itbfs_next(&it, &e)) {
    ak_ett_blob* eb =
      ak_da_at_impl(&ett_blobs, i);
    eb->e = e;
    eb->pt = ak_world_ett_parent(w, e);
    eb->order = ak_world_ett_order(w, e);

    ak_wv_itettcomp itc =
      ak_wv_itettcomp_make(&wv, e);
    uint32_t comp_count = 0;
    ak_comp_enum ce = 0;
    ak_comp_tu ctu = { 0 };
    while (
      ak_wv_itettcomp_next(&itc, &ctu)) {

      comp_count++;
    }

    i++;
  }

  ak_wv_itbfs_destroy(&it);

  ak_world_blob b = { 0 };
  return b;
}

void
ak_world_blob_destroy(ak_world_blob* wb);
