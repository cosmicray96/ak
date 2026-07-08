#include "ak/game/world/write.h"
#include "ak/coll/hmn.h"
#include "ak/debug.h"
#include "ak/game/comp.h"
#include "ak/game/core.h"
#include "ak/game/stg/world.h"
#include "ak/game/world/view.h"
#include "ak/game/world/view_itn.h"
#include "ak/system/stream.h"

#define s_version 11

#define try(x)                              \
  do {                                      \
    exiterr = x;                            \
    if (exiterr)                            \
      goto crash;                           \
  } while (0)

#define check(expr)                         \
  if (!(expr)) {                            \
    exiterr = ak_err_stm_badformat;         \
    goto crash;                             \
  }

ak_errcode
ak_stream_write_world(ak_stm stm,
                      ak_world* w,
                      ak_ett root,
                      ak_alct alct)
{
  ak_errcode exiterr = ak_ok;

  ak_stm_try(
    ak_stm_write_u32(stm, s_version));
  ak_stm_try(ak_stm_write_u32(
    stm,
    ak_world_ett_count_subtree(w, root)));

  bool first = true;

  ak_wv wv = ak_wv_make(w);
  ak_wv_itbfs it =
    ak_wv_itbfs_make(&wv, root, alct);
  ak_ett e = 0;
  while (1) {
    e = ak_wv_itbfs_next(&it);
    if (!e) {
      break;
    }

    try(ak_stm_write_u32(stm, e));
    if (first) {
      try(ak_stm_write_u32(stm, 0));
      first = false;
    } else {
      try(ak_stm_write_u32(
        stm, ak_world_ett_parent(w, e)));
    }
    try(ak_stm_write_u32(
      stm, ak_world_ett_compcount(w, e)));

    ak_comp_tu ctu = { 0 };
    ak_wv_itettcomp itc =
      ak_wv_itettcomp_make(&wv, e);
    while (
      ak_wv_itettcomp_next(&itc, &ctu)) {
      try(ak_stm_write_comp_tu(stm, ctu));
    }
  }

crash:
  ak_wv_itbfs_destroy(&it);
  ak_wv_destroy(&wv);
  return exiterr;
}

ak_errcode
ak_stream_read_world(ak_stm stm,
                     ak_world* o_w,
                     ak_alct alct)
{
  ak_errcode exiterr = ak_err;
  *o_w = ak_world_make(alct);

  uint32_t version = 0;
  try(ak_stm_read_u32(stm, &version));
  check(version == s_version);

  uint32_t ett_count = 0;
  try(ak_stm_read_u32(stm, &ett_count));

  for (uint32_t i = 0; i < ett_count; i++) {
    ak_ett e = 0;
    try(ak_stm_read_u32(stm, &e));
    ak_ett pt = 0;
    try(ak_stm_read_u32(stm, &pt));

    ak_world_ett_new(o_w, e, pt);

    uint32_t comp_count = 0;
    try(ak_stm_read_u32(stm, &comp_count));
    for (uint32_t j = 0; j < comp_count;
         j++) {
      ak_comp_tu ctu = { 0 };
      try(ak_stm_read_comp_tu(stm, &ctu));
      ak_world_comp_add_tu(o_w, e, &ctu);
    }
  }

exit:
  return exiterr;
crash:
  ak_world_destroy(o_w);
  goto exit;
}

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

void
ak_stream_print_world(ak_world* w)
{
  ak_wv wv = ak_wv_make(w);
  ak_ett root = ak_wv_ett_root(&wv);
  ak_wv_itdfspost it =
    ak_wv_itdfspost_make(&wv, root);
  ak_ett e = 0;
  while ((e = ak_wv_itdfspost_next(&it))) {
    ak_log("e: %d, pt: %d, cc: %d\n",
           e,
           ak_wv_ett_parent(&wv, e),
           ak_world_ett_compcount(w, e));
  }
}
