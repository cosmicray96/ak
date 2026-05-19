#include "ak/coll/fcnst.h"
#include "ak/coll/spa.h"
#include "ak/debug.h"

//===== metadata =====//
//--- private ---//
typedef struct
{
  uint32_t pt;
  uint32_t fc;
  uint32_t ns;
} metadata;

//===== ak_fcnst =====//
//--- private ---//
static void
remove_rec(ak_fcnst* t,
           uint32_t id,
           ak_fcnst_remove_fn remove_fn,
           void* remove_ctx)
{
  uint32_t c = ak_fcnst_fc(t, id);
  while (c) {
    uint32_t next = ak_fcnst_ns(t, c);
    remove_rec(t, c, remove_fn, remove_ctx);
    c = next;
  }
  if (remove_fn) {
    remove_fn(remove_ctx, id);
  }
  ak_spa_remove(&t->spa, id);
}

//--- export ---//
ak_fcnst
ak_fcnst_make(ak_alct alct)
{
  ak_fcnst t = { 0 };
  t.spa =
    ak_spa_make(sizeof(metadata), alct);
  t.root = 0;
  return t;
}

void
ak_fcnst_destroy(ak_fcnst* t)
{
  ak_spa_destroy(&t->spa);
  t->root = 0;
}

void
ak_fcnst_add(ak_fcnst* t,
             uint32_t id,
             uint32_t ptid)
{
  ak_assert(!ak_fcnst_exist(t, id));

  if (ptid == 0) {
    if (t->root == 0) {
      metadata md = { 0 };
      md.pt = 0;
      md.fc = 0;
      md.ns = 0;
      ak_spa_insert(&t->spa, id, &md);
      t->root = id;
      return;
    }
    ak_assert(false);
  }
  ak_assert(t->root);
  ak_assert(ak_fcnst_exist(t, ptid));

  metadata* ptmd = ak_spa_at(&t->spa, ptid);
  uint32_t fc = ptmd->fc;
  ptmd->fc = id;
  metadata md = { 0 };
  md.pt = ptid;
  md.fc = 0;
  md.ns = fc;
  ak_spa_insert(&t->spa, id, &md);
}

void
ak_fcnst_remove(ak_fcnst* t,
                uint32_t id,
                ak_fcnst_remove_fn remove_fn,
                void* remove_ctx)
{
  if (id == t->root) {
    remove_rec(t, id, remove_fn, remove_ctx);
    t->root = 0;
    return;
  }

  metadata* md = ak_spa_at(&t->spa, id);
  metadata* ptmd =
    ak_spa_at(&t->spa, md->pt);

  if (ptmd->fc == id) {
    ptmd->fc = md->ns;
  } else {
    uint32_t ps = ak_fcnst_ps(t, id);
    metadata* psmd = ak_spa_at(&t->spa, ps);
    psmd->ns = md->ns;
  }

  remove_rec(t, id, remove_fn, remove_ctx);
}

bool
ak_fcnst_exist(ak_fcnst* t, uint32_t id)
{
  return ak_spa_exist(&t->spa, id);
}

uint32_t
ak_fcnst_root(ak_fcnst* t)
{
  return t->root;
}

uint32_t
ak_fcnst_pt(ak_fcnst* t, uint32_t id)
{
  return ((metadata*)ak_spa_at(&t->spa, id))
    ->pt;
}
uint32_t
ak_fcnst_fc(ak_fcnst* t, uint32_t id)
{
  return ((metadata*)ak_spa_at(&t->spa, id))
    ->fc;
}
uint32_t
ak_fcnst_ns(ak_fcnst* t, uint32_t id)
{
  return ((metadata*)ak_spa_at(&t->spa, id))
    ->ns;
}

uint32_t
ak_fcnst_ps(ak_fcnst* t, uint32_t id)
{
  metadata* md = ak_spa_at(&t->spa, id);
  metadata* ptmd =
    ak_spa_at(&t->spa, md->pt);
  uint32_t c = ptmd->fc;

  if (c == id) {
    return 0;
  }

  while (c) {
    metadata* cmd = ak_spa_at(&t->spa, c);
    if (cmd->ns == id) {
      return c;
    }
    c = cmd->ns;
  }
  return 0;
}
