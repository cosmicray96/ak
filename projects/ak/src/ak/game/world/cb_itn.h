#ifndef ak_world_cb_itn_h
#define ak_world_cb_itn_h

#include "ak/core/mem/allocator.h"
#include "ak/game/comp.h"
#include "ak/game/stg/core.h"
#include "ak/game/stg/ettgen.h"
#include "ak/game/world/cb.h"

typedef enum
{
  cmd_none,
  cmd_ett_new,
  cmd_ett_remove,
  cmd_comp_add,
  cmd_comp_remove
} ak_worldcmd;
typedef struct
{
  ak_worldcmd c;
  ak_ett e;
  ak_comp_tu tu;
} ak_worldcmd_item;

ak_world_cb
ak_world_cb_make(ak_ettgen* eg,
                 ak_alct alct);
void
ak_world_cb_destroy(ak_world_cb* gc);

#endif
