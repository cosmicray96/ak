#include "ak/game/world/view.h"
#include "ak/game/world/view_itn.h"

//===== ak_world_v =====//
//--- internal ---//
ak_world_v
ak_world_v_make(ak_world* w)
{
  ak_world_v wv = { 0 };
  wv.w = w;
  return wv;
}
void
ak_world_v_destroy(ak_world_v* wv)
{
  wv->w = 0;
}

//--- export ---//
bool
ak_world_v_ett_exist(ak_world_v* wv,
                     ak_ett e)
{
  return ak_world_ett_exist(wv->w, e);
}

ak_ett
ak_world_v_ett_root(ak_world_v* wv)
{
  return ak_world_ett_root(wv->w);
}

ak_ett
ak_world_v_ett_parent(ak_world_v* wv,
                      ak_ett e)
{
  return ak_world_ett_parent(wv->w, e);
}

bool
ak_world_v_comp_exist(ak_world_v* wv,
                      ak_ett e,
                      ak_comp_enum ce)
{
  return ak_world_comp_exist(wv->w, e, ce);
}

#define X(name)                             \
  ak_##name##_t ak_world_v_comp_##name(     \
    ak_world_v* wv,                         \
    ak_ett e,                               \
    ak_comp_enum ce)                        \
  {                                         \
    return *(ak_##name##_t*)                \
      ak_world_comp_at(wv->w, e, ce);       \
  }

#include "ak/game/comp.inc"
#undef X
