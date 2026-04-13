#include "ak/coll/dq.h"
#include "ak/coll/dbuff.h"

//===== ak_dq =====//
#define s_gr 1.5f
//--- private ---//
static void
resize_ifneed(ak_dq* q)
{
  if (q->count < ak_dbuff_cap(&q->dbuff)) {
    return;
  }

  uint32_t cap = ak_dbuff_cap(&q->dbuff);
  uint32_t is = ak_dbuff_itemsize(&q->dbuff);
  ak_alct alct = ak_dbuff_alct(&q->dbuff);

  ak_dq newq =
    ak_dq_make(is, cap * s_gr, alct);

  void* item = ak_alct_alloc(alct, is);
  while (ak_dq_pop(q, item)) {
    ak_dq_push(&newq, item);
  }
  ak_alct_free(alct, item);

  ak_dq_destroy(q);
  *q = newq;
}

//--- export ---//
ak_dq
ak_dq_make(uint32_t itemsize,
           uint32_t starting_cap,
           ak_alct alct)
{
  ak_dq q = { 0 };
  q.dbuff = ak_dbuff_make(
    itemsize, s_gr, starting_cap, alct);
  q.ridx = 0;
  q.widx = 0;
  q.count = 0;
  return q;
}

void
ak_dq_destroy(ak_dq* q)
{
  ak_dbuff_destroy(&q->dbuff);
  q->ridx = 0;
  q->widx = 0;
  q->count = 0;
}

void
ak_dq_push(ak_dq* q, const void* item)
{
  resize_ifneed(q);

  ak_dbuff_overwrite(
    &q->dbuff, q->widx, item);
  q->widx =
    (q->widx + 1) % ak_dbuff_cap(&q->dbuff);
  q->count++;
}

bool
ak_dq_pop(ak_dq* q, void* item)
{
  if (q->count == 0) {
    return false;
  }

  ak_dbuff_at_copy(&q->dbuff, q->ridx, item);

  q->ridx =
    (q->ridx + 1) % ak_dbuff_cap(&q->dbuff);
  q->count--;
  return true;
}

void*
ak_dq_peek(ak_dq* q)
{
  if (q->count == 0) {
    return 0;
  }

  return ak_dbuff_at(&q->dbuff, q->ridx);
}
