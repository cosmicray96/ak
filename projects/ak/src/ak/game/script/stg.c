#include "ak/game/script/stg.h"

//===== ak_scriptstg =====//
//--- private ---//

#define ak_script_x(name)                   \
  ak_script ak_##name##_to_script();

#include "ak/game/script.inc"
#undef ak_script_x

struct ak_scriptstg
{
  ak_alct alct;
  ak_script scripts[ak_script_count_e];
};

//--- internal ---//
ak_scriptstg*
ak_scriptstg_make(ak_alct alct)
{
  ak_scriptstg* ss = ak_alct_alloc(
    alct, sizeof(ak_scriptstg));
  ss->alct = alct;

#define ak_script_x(name)                   \
  ss->scripts[ak_as_script_e(name)] =       \
    ak_##name##_to_script();

#include "ak/game/script.inc"
#undef ak_script_x

  return ss;
}

void
ak_scriptstg_destroy(ak_scriptstg* ss)
{
  ak_alct_free(ss->alct, ss);
}

ak_script
ak_scriptstg_at(ak_scriptstg* ss,
                ak_script_enum se)
{
  return ss->scripts[se];
}
