#include "ak/game/world/write.h"
#include "ak/core/io.h"
#include "ak/game/comp.h"
#include "ak/game/comp_write.h"
#include "ak/game/world/view.h"
#include "ak/game/world/view_itn.h"

void
ak_write_world(ak_iostream io,
               ak_world* w,
               ak_ett root,
               ak_alct alct)
{
  ak_write_u32(io, 10);
  ak_write_u32(io, ak_world_ett_count(w));

  ak_wv wv = ak_wv_make(w);
  ak_wv_itbfs it =
    ak_wv_itbfs_make(&wv, root, alct);
  ak_ett e = 0;
  while (ak_wv_itbfs_next(&it, &e)) {

    ak_write_u32(io, e);
    ak_write_u32(io,
                 ak_world_ett_parent(w, e));
    ak_write_u32(io,
                 ak_world_ett_order(w, e));
    ak_write_u32(
      io, ak_world_ett_compcount(w, e));

    ak_comp_tu ctu = { 0 };
    ak_wv_itettcomp itc =
      ak_wv_itettcomp_make(&wv, e);
    while (
      ak_wv_itettcomp_next(&itc, &ctu)) {
      ak_write_comp_tu(io, ctu);
    }
  }

  ak_wv_itbfs_destroy(&it);
  ak_wv_destroy(&wv);
}

void
ak_read_into_world(ak_iostream io,
                   ak_world* w,
                   ak_ett parent,
                   ak_alct alct)
{
  uint32_t version = ak_read_u32(io);
}
