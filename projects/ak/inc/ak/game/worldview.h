#ifndef ak_game_worldview
#define ak_game_worldview

#include "ak/export.h"
#include "ak/game/comp.h"
#include "ak/game/stg/core.h"

#include <stdbool.h>

typedef struct ak_worldview ak_worldview;

ak_ex bool
ak_worldview_ett_exist(ak_worldview* wv,
                       ak_ett e);
ak_ett
ak_worldview_ett_root(ak_worldview* w);
ak_ett
ak_worldview_ett_parent(ak_worldview* w,
                        ak_ett e);
ak_ett
ak_worldview_ett_firstchild(ak_worldview* w,
                            ak_ett e);
ak_ett
ak_worldview_ett_nextsib(ak_worldview* w,
                         ak_ett e);

bool
ak_worldview_comp_exist(ak_worldview* w,
                        ak_ett ett,
                        ak_comp_enum ce);
const void*
ak_worldview_comp_at(ak_worldview* w,
                     ak_ett ett,
                     ak_comp_enum ce);

#endif
