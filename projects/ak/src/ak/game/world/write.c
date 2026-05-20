#include "ak/game/world/write.h"
#include "ak/coll/da.h"
#include "ak/coll/dq.h"
#include "ak/coll/hmn.h"
#include "ak/core/io.h"
#include "ak/game/comp.h"
#include "ak/game/world/view.h"
#include "ak/game/world/view_itn.h"
#include "ak/system/idgen.h"

//--- private ---//
typedef struct
{
  ak_ett e;
  ak_comp_tu ctu;
} ett_comp_tu;

void
remap_ett(ak_comp_tu* ctu, ak_hmn* map)
{
  uint32_t offsets[16];
  uint32_t n =
    ak_comp_enum_offsets(ctu->ce, offsets);
  uint8_t* comp = ak_comp_tu_comp(ctu);
  for (uint32_t i = 0; i < n; i++) {
    ak_ett* e = (ak_ett*)(comp + offsets[i]);
    if (ak_hmn_exist(map, *e)) {
      ak_ett new_e =
        *(ak_ett*)ak_hmn_at(map, *e);
      *e = new_e;
    }
  }
}

//--- internal ---//
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
  while (1) {
    e = ak_wv_itbfs_next(&it);
    if (!e) {
      break;
    }

    ak_write_u32(io, e);
    ak_write_u32(io,
                 ak_world_ett_parent(w, e));
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
                   ak_idgen* ig,
                   ak_alct alct)
{
  ak_hmn map =
    ak_hmn_make(sizeof(ak_ett), alct);
  ak_dq comps =
    ak_dq_make(sizeof(ett_comp_tu), alct);

  uint32_t version = ak_read_u32(io);
  uint32_t ett_count = ak_read_u32(io);
  for (uint32_t i = 0; i < ett_count; i++) {
    ak_ett e = ak_read_u32(io);
    ak_ett pt = ak_read_u32(io);
    uint32_t compcount = ak_read_u32(io);

    if (i == 0) {
      ak_hmn_insert(&map, pt, &parent);
    }

    ak_ett new_e = ak_idgen_new(ig);
    ak_hmn_insert(&map, e, &new_e);

    ak_ett new_pt =
      *(ak_ett*)ak_hmn_at(&map, pt);

    ak_world_ett_new_last(w, new_e, new_pt);

    for (uint32_t j = 0; j < compcount;
         j++) {
      ett_comp_tu ectu = { 0 };
      ectu.e = new_e;
      ectu.ctu = ak_read_comp_tu(io);
      ak_dq_push(&comps, &ectu);
    }
  }

  ett_comp_tu ectu = { 0 };
  while (ak_dq_pop(&comps, &ectu)) {
    remap_ett(&ectu.ctu, &map);
    ak_world_comp_add_tu(
      w, ectu.e, &ectu.ctu);
  }

  ak_dq_destroy(&comps);
  ak_hmn_destroy(&map);
}
