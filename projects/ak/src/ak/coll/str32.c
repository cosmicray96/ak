#include "ak/coll/str32.h"
#include "ak/coll/da.h"
#include "ak/debug.h"

//===== ak_str32 =====//
//--- export ---//
ak_str32
ak_str32_make(ak_alct alct)
{
  ak_str32 s = { 0 };
  s.s =
    ak_da_make(sizeof(ak_codepoint), alct);
  return s;
}

bool
ak_str32_make_from_cstr(const char* cstr,
                        ak_alct alct,
                        ak_str32* o_s)
{
  *o_s = ak_str32_make(alct);
  const uint8_t* c = (const uint8_t*)cstr;
  while (*c) {
    ak_codepoint cp = { 0 };
    int32_t n =
      ak_codepoint_from_utf8(c, &cp);
    if (n < 0) {
      ak_str32_destroy(o_s);
      return false;
    }
    ak_str32_pushback(o_s, cp);
    c += n;
  }
  return true;
}

void
ak_str32_destroy(ak_str32* s)
{
  ak_da_destroy(&s->s);
}

uint32_t
ak_str32_count(const ak_str32* s)
{
  return ak_da_count(&s->s);
}

ak_codepoint
ak_str32_at(const ak_str32* s, uint32_t idx)
{
  ak_assert(idx < ak_str32_count(s));
  return *(ak_codepoint*)ak_da_at_const(
    &s->s, idx);
}

void
ak_str32_pushback(ak_str32* s,
                  ak_codepoint cp)
{
  ak_da_pushback(&s->s, &cp);
}

void
ak_str32_insert(ak_str32* s,
                uint32_t idx,
                ak_codepoint cp)
{
  ak_assert(idx < ak_str32_count(s));
  ak_da_insert(&s->s, idx, &cp);
}

void
ak_str32_overwrite(ak_str32* s,
                   uint32_t idx,
                   ak_codepoint cp)
{
  ak_assert(idx < ak_str32_count(s));
  ak_da_overwrite(&s->s, idx, &cp);
}

void
ak_str32_remove(ak_str32* s, uint32_t idx)
{
  ak_assert(idx < ak_str32_count(s));
  ak_da_remove(&s->s, idx);
}

void
ak_str32_remove_range(ak_str32* s,
                      uint32_t idx,
                      uint32_t count)
{
  ak_assert(idx + count <=
            ak_str32_count(s));
  ak_da_remove_range(&s->s, idx, count);
}

//===== ak_str32v =====//
//--- export ---//
ak_str32v
ak_str32v_make(ak_str32* s,
               uint32_t idx,
               uint32_t count)
{
  ak_str32v sv = { 0 };
  sv.s = s;
  sv.idx = idx;
  sv.count = count;
  return sv;
}

ak_str32v
ak_str32v_make_full(ak_str32* s)
{
  return ak_str32v_make(
    s, 0, ak_str32_count(s));
}

void
ak_str32v_pushback(ak_str32* s, ak_str32v sv)
{
  ak_da_bulk_pushback(
    &s->s, &sv.s->s, sv.idx, sv.count);
}

void
ak_str32v_insert(ak_str32* s,
                 uint32_t idx,
                 ak_str32v sv)
{
  ak_assert(false);
}

void
ak_str32v_overwrite(ak_str32* s,
                    uint32_t idx,
                    ak_str32v sv)
{
  ak_assert(false);
}
