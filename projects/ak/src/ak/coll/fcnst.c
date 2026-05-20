#include "ak/coll/fcnst.h"
#include "ak/coll/ds.h"
#include "ak/debug.h"
#include <stdint.h>

//===== metadata =====//
//--- private ---//
typedef struct
{
  ak_fcnstid pt;
  ak_fcnstid fc;
  ak_fcnstid ns;
  uint32_t depth;
  uint32_t ud;
} metadata;

//===== ak_fcnst =====//
//--- private ---//
static void
remove_rec(ak_fcnst* t,
           ak_fcnstid id,
           ak_fcnst_remove_fn remove_fn,
           void* remove_ctx)
{
  ak_fcnstid c = ak_fcnst_fc(t, id);
  while (c) {
    ak_fcnstid next = ak_fcnst_ns(t, c);
    remove_rec(t, c, remove_fn, remove_ctx);
    c = next;
  }
  if (remove_fn) {
    remove_fn(remove_ctx, id);
  }

  t->count--;
  ak_sla_remove(&t->slots, id);
}

//--- export ---//
ak_fcnst
ak_fcnst_make(ak_alct alct)
{
  ak_fcnst t = { 0 };
  t.slots =
    ak_sla_make(sizeof(metadata), alct);
  uint32_t id =
    ak_sla_insert_empty(&t.slots);
  ak_assert(id == 0);
  t.root = 0;
  return t;
}

void
ak_fcnst_destroy(ak_fcnst* t)
{
  ak_sla_destroy(&t->slots);
  t->root = 0;
}

ak_fcnstid
ak_fcnst_add(ak_fcnst* t,
             ak_fcnstid ptid,
             uint32_t ud)
{
  ak_fcnstid id = 0;
  metadata* md = 0;
  {
    metadata mditn = { 0 };
    mditn.pt = 0;
    mditn.fc = 0;
    mditn.ns = 0;
    mditn.depth = 0;
    mditn.ud = ud;
    id = ak_sla_insert(&t->slots, &mditn);
    md = ak_sla_at(&t->slots, id);
  }

  if (ptid == 0) {
    if (t->root == 0) {
      t->root = id;
      t->count++;
      return id;
    }
    ak_assert(false);
  }
  ak_assert(t->root);
  ak_assert(ak_fcnst_exist(t, ptid));

  metadata* ptmd =
    ak_sla_at(&t->slots, ptid);

  md->pt = ptid;
  md->depth = ptmd->depth + 1;

  ak_fcnstid fc = ptmd->fc;
  ptmd->fc = id;
  md->ns = fc;

  t->count++;
  return id;
}

ak_fcnstid
ak_fcnst_add_last(ak_fcnst* t,
                  ak_fcnstid ptid,
                  uint32_t ud)
{
  ak_fcnstid id = 0;
  metadata* md = 0;
  {
    metadata mditn = { 0 };
    mditn.pt = 0;
    mditn.fc = 0;
    mditn.ns = 0;
    mditn.depth = 0;
    mditn.ud = ud;
    id = ak_sla_insert(&t->slots, &mditn);
    md = ak_sla_at(&t->slots, id);
  }

  if (ptid == 0) {
    if (t->root == 0) {
      t->root = id;
      t->count++;
      return id;
    }
    ak_assert(false);
  }
  ak_assert(t->root);
  ak_assert(ak_fcnst_exist(t, ptid));

  metadata* ptmd =
    ak_sla_at(&t->slots, ptid);

  md->pt = ptid;
  md->depth = ptmd->depth + 1;

  if (!ptmd->fc) {
    ptmd->fc = id;
  } else {
    ak_fcnstid lc = ak_fcnst_lc(t, ptid);
    metadata* lcmd =
      ak_sla_at(&t->slots, lc);
    lcmd->ns = id;
  }

  t->count++;
  return id;
}

void
ak_fcnst_remove(ak_fcnst* t,
                ak_fcnstid id,
                ak_fcnst_remove_fn remove_fn,
                void* remove_ctx)
{
  if (id == t->root) {
    remove_rec(t, id, remove_fn, remove_ctx);
    t->root = 0;
    return;
  }

  metadata* md = ak_sla_at(&t->slots, id);
  metadata* ptmd =
    ak_sla_at(&t->slots, md->pt);

  if (ptmd->fc == id) {
    ptmd->fc = md->ns;
  } else {
    ak_fcnstid ps = ak_fcnst_ps(t, id);
    metadata* psmd =
      ak_sla_at(&t->slots, ps);
    psmd->ns = md->ns;
  }

  remove_rec(t, id, remove_fn, remove_ctx);
}

bool
ak_fcnst_exist(ak_fcnst* t, ak_fcnstid id)
{
  return ak_sla_exist(&t->slots, id);
}

ak_fcnstid
ak_fcnst_root(ak_fcnst* t)
{
  return t->root;
}

uint32_t
ak_fcnst_order(ak_fcnst* t, ak_fcnstid id)
{
  uint32_t order = 0;
  ak_fcnstid c =
    ak_fcnst_fc(t, ak_fcnst_pt(t, id));
  while (c != id) {
    order++;
    c = ak_fcnst_ns(t, c);
  }
  return order;
}

uint32_t
ak_fcnst_depth(ak_fcnst* t, ak_fcnstid id)
{
  return ((metadata*)ak_sla_at(&t->slots,
                               id))
    ->depth;
}
uint32_t
ak_fcnst_ud(ak_fcnst* t, ak_fcnstid id)
{
  return ((metadata*)ak_sla_at(&t->slots,
                               id))
    ->ud;
}

ak_fcnstid
ak_fcnst_pt(ak_fcnst* t, ak_fcnstid id)
{
  return ((metadata*)ak_sla_at(&t->slots,
                               id))
    ->pt;
}
ak_fcnstid
ak_fcnst_fc(ak_fcnst* t, ak_fcnstid id)
{
  return ((metadata*)ak_sla_at(&t->slots,
                               id))
    ->fc;
}
ak_fcnstid
ak_fcnst_ns(ak_fcnst* t, ak_fcnstid id)
{
  return ((metadata*)ak_sla_at(&t->slots,
                               id))
    ->ns;
}

ak_fcnstid
ak_fcnst_ps(ak_fcnst* t, ak_fcnstid id)
{
  metadata* md = ak_sla_at(&t->slots, id);
  metadata* ptmd =
    ak_sla_at(&t->slots, md->pt);
  ak_fcnstid c = ptmd->fc;

  if (c == id) {
    return 0;
  }

  while (c) {
    metadata* cmd = ak_sla_at(&t->slots, c);
    if (cmd->ns == id) {
      return c;
    }
    c = cmd->ns;
  }
  return 0;
}

ak_fcnstid
ak_fcnst_lc(ak_fcnst* t, ak_fcnstid ptid)
{
  ak_fcnstid c = ak_fcnst_fc(t, ptid);
  while (c) {
    ak_fcnstid next = ak_fcnst_ns(t, c);
    if (!next) {
      return c;
    }
    c = next;
  }
  return 0;
}

ak_fcnstid
ak_fcnst_leftmost(ak_fcnst* t, ak_fcnstid id)
{
  ak_fcnstid c = ak_fcnst_fc(t, id);
  while (c) {
    id = c;
    c = ak_fcnst_fc(t, id);
  }
  return id;
}

//===== ak_fcnst_itchild =====//
//--- export ---//
ak_fcnst_itchild
ak_fcnst_itchild_make(ak_fcnst* t,
                      ak_fcnstid pt)
{
  ak_fcnst_itchild it = { 0 };
  it.t = t;
  it.last = ak_fcnst_fc(t, pt);
  it.started = false;
  return it;
}

ak_fcnstid
ak_fcnst_itchild_next(ak_fcnst_itchild* it)
{
  if (!it->started) {
    it->started = true;
    return it->last;
  }
  it->last = ak_fcnst_ns(it->t, it->last);

  return it->last;
}

//===== ak_fcnst_itancestor =====//
//--- export ---//
ak_fcnst_itancestor
ak_fcnst_itancestor_make(ak_fcnst* t,
                         ak_fcnstid id)
{
  ak_fcnst_itancestor it = { 0 };
  it.t = t;
  it.id = id;
  return it;
}

ak_fcnstid
ak_fcnst_itancestor_next(
  ak_fcnst_itancestor* it)
{
  if (!it->id) {
    return 0;
  }
  ak_fcnstid id = it->id;
  it->id = ak_fcnst_pt(it->t, it->id);
  return id;
}

//===== ak_fcnst_itdfspre =====//
//--- export ---//
ak_fcnst_itdfspost
ak_fcnst_itdfspost_make(ak_fcnst* t,
                        ak_fcnstid root)
{
  ak_fcnst_itdfspost it = { 0 };
  it.t = t;
  it.root = root;
  it.last = ak_fcnst_leftmost(t, root);
  it.started = false;
  return it;
}

ak_fcnstid
ak_fcnst_itdfspost_next(
  ak_fcnst_itdfspost* it)
{
  if (!it->started) {
    it->started = true;
    return it->last;
  }

  if (it->last == it->root) {
    return 0;
  }

  ak_fcnstid ns =
    ak_fcnst_ns(it->t, it->last);
  if (ns) {
    it->last = ak_fcnst_leftmost(it->t, ns);
  } else {
    it->last = ak_fcnst_pt(it->t, it->last);
  }
  return it->last;
}

//===== ak_fcnst_itdfspre =====//
//--- export ---//
ak_fcnst_itdfspre
ak_fcnst_itdfspre_make(ak_fcnst* t,
                       ak_fcnstid root,
                       ak_alct alct)
{
  ak_fcnst_itdfspre it = { 0 };
  it.t = t;
  it.s =
    ak_ds_make(sizeof(ak_fcnstid), alct);
  ak_ds_push(&it.s, &root);
  return it;
}

void
ak_fcnst_itdfspre_destroy(
  ak_fcnst_itdfspre* it)
{
  ak_ds_destroy(&it->s);
  it->t = 0;
}

void
ak_fcnst_itdfspre_reset(
  ak_fcnst_itdfspre* it,
  ak_fcnstid root)
{
  ak_ds_clear(&it->s);
  ak_ds_push(&it->s, &root);
}

ak_fcnstid
ak_fcnst_itdfspre_next(ak_fcnst_itdfspre* it)
{
  ak_fcnstid id = 0;
  if (!ak_ds_pop(&it->s, &id)) {
    return 0;
  }

  ak_fcnstid ns = ak_fcnst_ns(it->t, id);
  if (ns) {
    ak_ds_push(&it->s, &ns);
  }

  ak_fcnstid fc = ak_fcnst_fc(it->t, id);
  if (fc) {
    ak_ds_push(&it->s, &fc);
  }

  return id;
}

//===== ak_fcnst_itbfs =====//
//--- export ---//
ak_fcnst_itbfs
ak_fcnst_itbfs_make(ak_fcnst* t,
                    ak_fcnstid root,
                    ak_alct alct)
{
  ak_fcnst_itbfs it = { 0 };
  it.t = t;
  it.q =
    ak_dq_make(sizeof(ak_fcnstid), alct);
  ak_dq_push(&it.q, &root);
  return it;
}

void
ak_fcnst_itbfs_destroy(ak_fcnst_itbfs* it)
{
  ak_dq_destroy(&it->q);
  it->t = 0;
}

void
ak_fcnst_itbfs_reset(ak_fcnst_itbfs* it,
                     ak_fcnstid root)
{
  ak_dq_clear(&it->q);
  ak_dq_push(&it->q, &root);
}

ak_fcnstid
ak_fcnst_itbfs_next(ak_fcnst_itbfs* it)
{
  ak_fcnstid id = 0;
  if (!ak_dq_pop(&it->q, &id)) {
    return 0;
  }
  ak_fcnst_itchild cit =
    ak_fcnst_itchild_make(it->t, id);
  while (true) {
    ak_fcnstid c =
      ak_fcnst_itchild_next(&cit);
    if (!c) {
      break;
    }
    ak_dq_push(&it->q, &c);
  }

  return id;
}

//===== ak_fcnst_itleaf =====//
//--- export ---//
ak_fcnst_itleaf
ak_fcnst_itleaf_make(ak_fcnst* t,
                     ak_fcnstid root)
{
  ak_fcnst_itleaf it = { 0 };
  it.dfsit =
    ak_fcnst_itdfspost_make(t, root);
  return it;
}

ak_fcnstid
ak_fcnst_itleaf_next(ak_fcnst_itleaf* it)
{
  while (true) {
    ak_fcnstid id =
      ak_fcnst_itdfspost_next(&it->dfsit);
    if (!id) {
      return 0;
    }
    if (!ak_fcnst_fc(it->dfsit.t, id)) {
      return id;
    }
  }
  return 0;
}
