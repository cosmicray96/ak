#ifndef ak_world_cb_itn_h
#define ak_world_cb_itn_h

#include "ak/core/mem/allocator.h"
#include "ak/game/comp.h"
#include "ak/game/core.h"
#include "ak/game/world/cb.h"
#include "ak/system/idgen.h"

typedef enum
{
  ak_wcbtype_ett_new,
  ak_wcbtype_ett_remove,
  ak_wcbtype_comp_add,
  ak_wcbtype_comp_remove
} ak_wcbtype;

typedef struct
{
  ak_wcbtype cmd;
  ak_ett e;
  ak_ett pt;
  ak_comp_tu ctu;
} ak_wcbitem;

struct ak_wcb
{
  ak_alct alct;
  ak_idgen* ig;
  ak_dq cmds;
};

ak_wcb
ak_wcb_make(ak_idgen* ig, ak_alct alct);
void
ak_wcb_destroy(ak_wcb* gc);

uint32_t
ak_wcb_count(ak_wcb* wcb);
bool
ak_wcb_peek(ak_wcb* wcb,
            uint32_t idx,
            ak_wcbitem* o_item);
bool
ak_wcb_pop(ak_wcb* wcb, ak_wcbitem* o_item);

void
ak_wcb_joinback(ak_wcb* dest, ak_wcb* src);

#endif
