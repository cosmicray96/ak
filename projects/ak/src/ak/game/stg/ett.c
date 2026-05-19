#include "ak/game/stg/ett.h"
#include "ak/coll/spa.h"
#include "ak/debug.h"
#include "ak/game/core.h"

//===== ak_ettstg =====//
//--- private ---//
typedef struct
{
  ak_ett pt;
  ak_ett fc;
  ak_ett ns;
  uint32_t depth;
} ettitn;

static ettitn*
get_ettitn(ak_ettstg* es, ak_ett e)
{
  ak_assert(ak_spa_exist(&es->etts, e));
  return ak_spa_at(&es->etts, e);
}

static void
new_ettitn(ak_ettstg* es,
           ak_ett e,
           const ettitn* eitn)
{
  ak_assert(!ak_spa_exist(&es->etts, e));
  ak_spa_insert(&es->etts, e, eitn);
}

static void
remove_ettitn(ak_ettstg* es,
              ak_ett e,
              ak_ettstg_remove_fn remove_fn,
              void* remove_ctx)
{
  ak_ett c = ak_ettstg_firstchild(es, e);
  while (c) {
    ak_ett next = ak_ettstg_nextsib(es, c);
    remove_ettitn(
      es, c, remove_fn, remove_ctx);
    c = next;
  }

  if (remove_fn) {
    remove_fn(remove_ctx, e);
  }
  ak_spa_remove(&es->etts, e);
}

//--- public ---//
ak_ettstg
ak_ettstg_make(ak_ett root, ak_alct alct)
{
  ak_assert(root);

  ak_ettstg es = { 0 };
  es.etts =
    ak_spa_make(sizeof(ettitn), alct);
  es.root = root;
  {
    ettitn ritn = { 0 };
    ritn.depth = 0;
    ak_spa_insert(&es.etts, root, &ritn);
  }

  return es;
}

void
ak_ettstg_destroy(ak_ettstg* es)
{
  ak_spa_destroy(&es->etts);
  es->root = 0;
}

uint32_t
ak_ettstg_count(ak_ettstg* es)
{
  return ak_spa_count(&es->etts);
}

void
ak_ettstg_newett(ak_ettstg* es,
                 ak_ett e,
                 ak_ett pt)
{
  ak_assert(!ak_ettstg_exist(es, e));

  ettitn* ptitn = get_ettitn(es, pt);

  ettitn eitn = { 0 };
  eitn.pt = pt;
  eitn.fc = 0;
  eitn.ns = 0;
  eitn.depth = ptitn->depth + 1;

  if (ptitn->fc) {
    eitn.ns = ptitn->fc;
  }
  ptitn->fc = e;

  new_ettitn(es, e, &eitn);
}

void
ak_ettstg_newett_last(ak_ettstg* es,
                      ak_ett e,
                      ak_ett pt)
{

  ak_assert(!ak_ettstg_exist(es, e));

  ettitn* ptitn = get_ettitn(es, pt);

  ettitn eitn = { 0 };
  eitn.pt = pt;
  eitn.fc = 0;
  eitn.ns = 0;
  eitn.depth = ptitn->depth + 1;

  if (!ptitn->fc) {
    ptitn->fc = e;
    new_ettitn(es, e, &eitn);
    return;
  }
  ak_ett sib = ptitn->fc;
  while (true) {
    ak_ett next = ak_ettstg_nextsib(es, sib);
    if (!next) {
      break;
    }
    sib = next;
  }

  ettitn* sibitn = get_ettitn(es, sib);
  sibitn->ns = e;
  new_ettitn(es, e, &eitn);
}

ak_ett
ak_ettstg_root(ak_ettstg* es)
{
  return es->root;
}

uint32_t
ak_ettstg_depth(ak_ettstg* es, ak_ett e)
{
  return get_ettitn(es, e)->depth;
}

uint32_t
ak_ettstg_order(ak_ettstg* es, ak_ett e)
{
  ak_ett pt = ak_ettstg_parent(es, e);
  if (!pt) {
    return 0;
  }
  ak_ett c = ak_ettstg_firstchild(es, pt);
  uint32_t order = 0;
  while (c) {
    if (c == e) {
      return order;
    }
    order++;
    c = ak_ettstg_nextsib(es, c);
  }
  ak_assert(false);
  return order;
}

bool
ak_ettstg_exist(ak_ettstg* es, ak_ett e)
{
  return ak_spa_exist(&es->etts, e);
}

ak_ett
ak_ettstg_parent(ak_ettstg* es, ak_ett e)
{
  return get_ettitn(es, e)->pt;
}

ak_ett
ak_ettstg_firstchild(ak_ettstg* es, ak_ett e)
{
  return get_ettitn(es, e)->fc;
}
ak_ett
ak_ettstg_nextsib(ak_ettstg* es, ak_ett e)
{
  return get_ettitn(es, e)->ns;
}

ak_ett
ak_ettstg_prevsib(ak_ettstg* es, ak_ett e)
{
  ak_assert(e != es->root);

  ettitn* eitn = get_ettitn(es, e);
  ettitn* ptitn = get_ettitn(es, eitn->pt);
  ak_ett c = ptitn->fc;

  if (c == e) {
    return 0;
  }

  while (c) {
    ettitn* citn = get_ettitn(es, c);
    if (citn->ns == e) {
      return c;
    }
    c = citn->ns;
  }
  return 0;
}

void
ak_ettstg_remove_cb(
  ak_ettstg* es,
  ak_ett e,
  ak_ettstg_remove_fn remove_fn,
  void* remove_ctx)
{
  ak_assert(e != es->root);

  ettitn* eitn = get_ettitn(es, e);
  ak_ett pt = eitn->pt;
  ettitn* ptitn = get_ettitn(es, pt);
  if (e == ptitn->fc) {
    ptitn->fc = eitn->ns;
  } else {
    ak_ett ps = ak_ettstg_prevsib(es, e);
    ak_assert(ps);
    ettitn* psitn = get_ettitn(es, ps);
    psitn->ns = eitn->ns;
  }

  eitn->pt = 0;
  eitn->ns = 0;
  remove_ettitn(
    es, e, remove_fn, remove_ctx);
}

ak_ett
ak_ettstg_leftmost(ak_ettstg* es, ak_ett e)
{
  ak_ett c = ak_ettstg_firstchild(es, e);
  while (c) {
    e = c;
    c = ak_ettstg_firstchild(es, e);
  }
  return e;
}

//===== ak_ettstg_itdfs =====//
//--- public ---//
ak_ettstg_itdfs
ak_ettstg_itdfs_make(ak_ettstg* es,
                     ak_ett root)
{
  ak_ettstg_itdfs it = { 0 };
  it.es = es;
  it.root = root;
  it.last = ak_ettstg_leftmost(es, root);
  it.started = false;
  return it;
}

bool
ak_ettstg_itdfs_next(ak_ettstg_itdfs* it,
                     ak_ett* o_e)
{
  if (!it->started) {
    it->started = true;
    *o_e = it->last;
    return true;
  }

  if (it->last == it->root) {
    return false;
  }

  ak_ett ns =
    ak_ettstg_nextsib(it->es, it->last);
  if (ns) {
    it->last =
      ak_ettstg_leftmost(it->es, ns);
  } else {
    it->last =
      ak_ettstg_parent(it->es, it->last);
  }

  *o_e = it->last;
  return true;
}
