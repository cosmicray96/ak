#include "ak/coll/str.h"
#include "ak/debug.h"

//===== ak_str =====//
//--- private ---//
static void
nullterm_push(ak_str* s)
{
  char ch = '\0';
  ak_da_pushback(&s->da, &ch);
}
static void
nullterm_pop(ak_str* s)
{
  ak_da_remove(&s->da,
               ak_da_count(&s->da) - 1);
}

//--- export ---//
ak_str
ak_str_make(ak_alct alct)
{
  ak_str s = { 0 };
  s.da = ak_da_make(sizeof(char), alct);
  nullterm_push(&s);
  return s;
}

ak_str
ak_str_from_cstr(const char* cstr,
                 ak_alct alct)
{
  ak_str s = ak_str_make(alct);
  ak_str_cstr_pushback(&s, cstr);
  return s;
}

void
ak_str_destroy(ak_str* s)
{
  ak_da_destroy(&s->da);
}

void
ak_str_clear(ak_str* s)
{
  ak_da_clear(&s->da);
  nullterm_push(s);
}

uint32_t
ak_str_count(const ak_str* s)
{
  return ak_da_count(&s->da) - 1;
}

char
ak_str_at(const ak_str* s, uint32_t idx)
{
  ak_assert(idx < ak_str_count(s));
  const char* ch =
    ak_da_at_const(&s->da, idx);
  return *ch;
}

void
ak_str_insert(ak_str* s,
              uint32_t idx,
              char ch)
{
  ak_assert(idx < ak_str_count(s));
  ak_da_insert(&s->da, idx, &ch);
}

void
ak_str_overwrite(ak_str* s,
                 uint32_t idx,
                 char ch)
{
  ak_assert(idx < ak_str_count(s));
  ak_da_overwrite(&s->da, idx, &ch);
}

void
ak_str_pushback(ak_str* s, char ch)
{
  nullterm_pop(s);
  ak_da_pushback(&s->da, &ch);
  nullterm_push(s);
}

void
ak_str_remove(ak_str* s, uint32_t idx)
{
  ak_assert(idx < ak_str_count(s));
  ak_da_remove(&s->da, idx);
}

void
ak_str_remove_range(ak_str* s,
                    uint32_t idx,
                    uint32_t count)
{
  ak_assert(idx < ak_str_count(s));
  ak_assert(idx + count <= ak_str_count(s));
  ak_da_remove_range(&s->da, idx, count);
}

void
ak_str_cstr_insert(ak_str* s,
                   uint32_t idx,
                   const char* cstr);

void
ak_str_cstr_overwrite(ak_str* s,
                      uint32_t idx,
                      const char* cstr);

void
ak_str_cstr_pushback(ak_str* s,
                     const char* cstr)
{
  nullterm_pop(s);
  char ch = 0;
  while ((ch = *cstr)) {
    ak_da_pushback(&s->da, &ch);
    cstr++;
  }
  nullterm_push(s);
}

void
ak_str_str_pushback(ak_str* dest,
                    const ak_str* src)
{
  ak_str_cstr_pushback(
    dest, ak_str_ptr_const(src));
}
