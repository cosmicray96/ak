/*
#include "ak/game/sys/ani.h"
#include "ak/core/math/vec4f.h"
#include "ak/game/comp.h"
#include "ak/game/core.h"
#include "ak/game/world/cb.h"
#include "ak/game/world/view.h"
#include "ak/os/time.h"
#include "ak/res/reg.h"
#include "ak/res/reses/aniclip.h"

ak_sys_ani
ak_sys_ani_make(ak_alct alct)
{
  ak_sys_ani sa;
  return sa;
}

void
ak_sys_ani_destroy(ak_sys_ani* sani)
{
  // empty
}

void
ak_sys_ani_update(ak_sys_ani* a,
                  ak_wv* wv,
                  ak_wcb* wcb,
                  ak_resreg* rr,
                  ak_dur delta)
{
  ak_ett root = ak_wv_ett_root(wv);
  if (!root) {
    return;
  }

  ak_wv_itcomp it =
    ak_wv_itcomp_make(wv, ak_anistate_e);

  while (true) {
    ak_anistate_t as = { 0 };
    ak_ett e = ak_wv_itcomp_next(&it, &as);
    if (!e) {
      break;
    }

    ak_quadsimple_t qs =
      ak_wv_comp_quadsimple(wv, e);

    as.curtime += ak_dur_as_secs_f(delta);

    ak_aniclip ac = ak_resreg_get_aniclip(
      rr, as.aniclipid);
    uint32_t count = ak_aniclip_count(&ac);
    uint32_t frame_idx =
      as.curtime / as.framedur;
    frame_idx = frame_idx % count;

    uint32_t frame =
      ak_aniclip_at(&ac, frame_idx);

    ak_texatlas ta = ak_resreg_get_texatlas(
      rr, ak_aniclip_texatlas(&ac));
    ak_vec4f uv_rect =
      ak_texatlas_at(&ta, frame);

    qs.data.uv_min.x = uv_rect.x;
    qs.data.uv_min.y = uv_rect.y;
    qs.data.uv_max.x = uv_rect.z;
    qs.data.uv_max.y = uv_rect.w;

    ak_wcb_comp_anistate_add(wcb, e, as);
    ak_wcb_comp_quadsimple_add(wcb, e, qs);
  }
}
*/
