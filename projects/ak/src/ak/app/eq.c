#include "ak/app/eq.h"
#include "ak/coll/dq.h"

ak_app_eq
ak_app_eq_make(ak_alct alct)
{
  ak_app_eq eq;
  eq.events =
    ak_dq_make(sizeof(ak_evt), alct);
  return eq;
}
void
ak_app_eq_destroy(ak_app_eq* eq)
{
  ak_dq_destroy(&eq->events);
}

void
ak_app_eq_push(ak_app_eq* eq, ak_evt e)
{
  ak_dq_push(&eq->events, &e);
}

ak_evt
ak_app_eq_pop(ak_app_eq* eq)
{
  ak_evt e = { 0 };
  bool success = ak_dq_pop(&eq->events, &e);
  if (success) {
    return e;
  }
  e.type = ak_evt_none;
  return e;
}
