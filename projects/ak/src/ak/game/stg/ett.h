#ifndef ak_game_stg_ett_h
#define ak_game_stg_ett_h

#include "ak/coll/spa.h"
#include "ak/game/stg/core.h"

#include <stdbool.h>

//===== ak_ettstg =====//
typedef struct
{
  ak_spa etts;
  ak_ett root;
} ak_ettstg;

ak_ettstg
ak_ettstg_make(ak_ett root, ak_alct alct);
void
ak_ettstg_destroy(ak_ettstg* es);

void
ak_ettstg_newett(ak_ettstg* es,
                 ak_ett e,
                 ak_ett pt);

ak_ett
ak_ettstg_root(ak_ettstg* es);
bool
ak_ettstg_exist(ak_ettstg* es, ak_ett e);

ak_ett
ak_ettstg_parent(ak_ettstg* es, ak_ett e);
ak_ett
ak_ettstg_firstchild(ak_ettstg* es,
                     ak_ett e);
ak_ett
ak_ettstg_nextsib(ak_ettstg* es, ak_ett e);

void
ak_ettstg_remove(ak_ettstg* es, ak_ett e);

//===== ak_ettstg_itdfs =====//
typedef struct
{
  ak_ettstg* es;
  ak_ett root;
  ak_ett last;
  bool started;
} ak_ettstg_itdfs;

ak_ettstg_itdfs
ak_ettstg_itdfs_make(ak_ettstg* es,
                     ak_ett root);

bool
ak_ettstg_itdfs_next(ak_ettstg_itdfs* it,
                     ak_ett* o_e);

#endif
