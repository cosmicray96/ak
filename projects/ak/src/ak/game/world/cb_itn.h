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

struct ak_wcb
{
  ak_alct alct;
  ak_ettgen* eg;
  ak_dq cmds;
};

ak_wcb
ak_wcb_make(ak_ettgen* eg, ak_alct alct);
void
ak_wcb_destroy(ak_wcb* gc);

uint32_t
ak_wcb_count(ak_wcb* wcb);
bool
ak_wcb_peek(ak_wcb* wcb,
            uint32_t idx,
            ak_world_cmditem* o_item);
bool
ak_wcb_pop(ak_wcb* wcb,
           ak_world_cmditem* o_item);

#endif
