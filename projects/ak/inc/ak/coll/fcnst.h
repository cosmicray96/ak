#ifndef ak_coll_fcnst_h
#define ak_coll_fcnst_h

#include "ak/coll/dq.h"
#include "ak/coll/ds.h"
#include "ak/coll/sla.h"
#include "ak/core/mem/allocator.h"
#include <stdbool.h>

typedef uint32_t ak_fcnstid;
typedef struct
{
  ak_sla slots;
  uint32_t root;
  uint32_t count;
} ak_fcnst;

ak_fcnst
ak_fcnst_make(ak_alct alct);
void
ak_fcnst_destroy(ak_fcnst* t);

static uint32_t
ak_fcnst_count(ak_fcnst* t)
{
  return t->count;
}

ak_fcnstid
ak_fcnst_add(ak_fcnst* t,
             ak_fcnstid ptid,
             uint32_t ud);
ak_fcnstid
ak_fcnst_add_last(ak_fcnst* t,
                  ak_fcnstid ptid,
                  uint32_t ud);

typedef void (*ak_fcnst_remove_fn)(
  void* ctx,
  ak_fcnstid id);
void
ak_fcnst_remove(ak_fcnst* t,
                ak_fcnstid id,
                ak_fcnst_remove_fn remove_fn,
                void* ctx);

ak_fcnstid
ak_fcnst_root(ak_fcnst* t);
bool
ak_fcnst_exist(ak_fcnst* t, ak_fcnstid id);

uint32_t
ak_fcnst_order(ak_fcnst* t, ak_fcnstid id);
uint32_t
ak_fcnst_depth(ak_fcnst* t, ak_fcnstid id);
uint32_t
ak_fcnst_ud(ak_fcnst* t, ak_fcnstid id);
ak_fcnstid
ak_fcnst_pt(ak_fcnst* t, ak_fcnstid id);
ak_fcnstid
ak_fcnst_fc(ak_fcnst* t, ak_fcnstid id);
ak_fcnstid
ak_fcnst_ns(ak_fcnst* t, ak_fcnstid id);
ak_fcnstid
ak_fcnst_ps(ak_fcnst* t, ak_fcnstid id);
ak_fcnstid
ak_fcnst_lc(ak_fcnst* t, ak_fcnstid ptid);

ak_fcnstid
ak_fcnst_leftmost(ak_fcnst* t,
                  ak_fcnstid id);

//===== ak_fcnst_itchild =====//
typedef struct
{
  ak_fcnst* t;
  ak_fcnstid last;
  bool started;
} ak_fcnst_itchild;
ak_fcnst_itchild
ak_fcnst_itchild_make(ak_fcnst* t,
                      ak_fcnstid pt);
ak_fcnstid
ak_fcnst_itchild_next(ak_fcnst_itchild* it);

//===== ak_fcnst_itancestor =====//
typedef struct
{
  ak_fcnst* t;
  ak_fcnstid id;
} ak_fcnst_itancestor;
ak_fcnst_itancestor
ak_fcnst_itancestor_make(ak_fcnst* t,
                         ak_fcnstid id);
ak_fcnstid
ak_fcnst_itancestor_next(
  ak_fcnst_itancestor* it);

//===== ak_fcnst_itdfspost =====//
typedef struct
{
  ak_fcnst* t;
  ak_fcnstid root;
  ak_fcnstid last;
  bool started;
} ak_fcnst_itdfspost;
ak_fcnst_itdfspost
ak_fcnst_itdfspost_make(ak_fcnst* t,
                        ak_fcnstid root);
ak_fcnstid
ak_fcnst_itdfspost_next(
  ak_fcnst_itdfspost* it);

typedef struct
{
  ak_fcnst* t;
  ak_ds s;
} ak_fcnst_itdfspre;
ak_fcnst_itdfspre
ak_fcnst_itdfspre_make(ak_fcnst* t,
                       ak_fcnstid root,
                       ak_alct alct);
void
ak_fcnst_itdfspre_destroy(
  ak_fcnst_itdfspre* it);
void
ak_fcnst_itdfspre_reset(
  ak_fcnst_itdfspre* it,
  ak_fcnstid root);
ak_fcnstid
ak_fcnst_itdfspre_next(
  ak_fcnst_itdfspre* it);

//===== ak_fcnst_itbfs =====//
typedef struct
{
  ak_fcnst* t;
  ak_dq q;
} ak_fcnst_itbfs;
ak_fcnst_itbfs
ak_fcnst_itbfs_make(ak_fcnst* t,
                    ak_fcnstid root,
                    ak_alct alct);
void
ak_fcnst_itbfs_destroy(ak_fcnst_itbfs* it);
void
ak_fcnst_itbfs_reset(ak_fcnst_itbfs* it,
                     ak_fcnstid root);
ak_fcnstid
ak_fcnst_itbfs_next(ak_fcnst_itbfs* it);

//===== ak_fcnst_itleaf =====//
typedef struct
{
  ak_fcnst_itdfspost dfsit;
} ak_fcnst_itleaf;
ak_fcnst_itleaf
ak_fcnst_itleaf_make(ak_fcnst* t,
                     ak_fcnstid root);
ak_fcnstid
ak_fcnst_itleaf_next(ak_fcnst_itleaf* it);

#endif
