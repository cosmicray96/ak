#ifndef ak_world_cb_itn_h
#define ak_world_cb_itn_h

#include "ak/core/mem/allocator.h"
#include "ak/game/comp.h"
#include "ak/game/stg/core.h"
#include "ak/game/stg/ettgen.h"
#include "ak/game/world/cb.h"

typedef enum
{
  ak_world_cmd_ett_new,
  ak_world_cmd_ett_remove,
  ak_world_cmd_comp_modify,
  ak_world_cmd_comp_add,
  ak_world_cmd_comp_remove
} ak_world_cmd;
typedef struct
{
  ak_world_cmd cmd;
  ak_ett e;
  ak_ett pt;
  ak_comp_tu ctu;
} ak_world_cmditem;

struct ak_world_cb
{
  ak_alct alct;
  ak_ettgen* eg;
  ak_dq cmds;
};

ak_world_cb
ak_world_cb_make(ak_ettgen* eg,
                 ak_alct alct);
void
ak_world_cb_destroy(ak_world_cb* gc);

uint32_t
ak_world_cb_count(ak_world_cb* wcb);
bool
ak_world_cb_peek(ak_world_cb* wcb,
                 uint32_t idx,
                 ak_world_cmditem* o_item);
bool
ak_world_cb_pop(ak_world_cb* wcb,
                ak_world_cmditem* o_item);

#endif
