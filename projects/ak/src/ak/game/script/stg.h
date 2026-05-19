#ifndef ak_game_script_stg_h
#define ak_game_script_stg_h

#include "ak/core/mem/allocator.h"
#include "ak/game/script.h"

typedef struct ak_scriptstg ak_scriptstg;

ak_scriptstg*
ak_scriptstg_make(ak_alct alct);

void
ak_scriptstg_destroy(ak_scriptstg* ss);

ak_script
ak_scriptstg_at(ak_scriptstg* ss,
                ak_script_enum se);

#endif
