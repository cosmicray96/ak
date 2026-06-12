#ifndef ak_coll_str_h
#define ak_coll_str_h

#include "ak/coll/da.h"
#include "ak/core/mem/allocator.h"
#include "ak/export.h"

typedef struct
{
  ak_da da;
} ak_str;

ak_ex ak_str
ak_str_make(ak_alct alct);
ak_ex ak_str
ak_str_from_cstr(const char* cstr,
                 ak_alct alct);
ak_ex void
ak_str_destroy(ak_str* s);

ak_ex void
ak_str_clear(ak_str* s);

ak_ex uint32_t
ak_str_count(const ak_str* s);

static char*
ak_str_ptr(ak_str* s)
{
  return ak_da_ptr(&s->da);
}
static const char*
ak_str_ptr_const(const ak_str* s)
{
  return ak_da_ptr_const(&s->da);
}

ak_ex char
ak_str_at(const ak_str* s, uint32_t idx);

ak_ex void

ak_str_insert(ak_str* s,
              uint32_t idx,
              char ch);
ak_ex void
ak_str_overwrite(ak_str* s,
                 uint32_t idx,
                 char ch);
ak_ex void
ak_str_pushback(ak_str* s, char ch);

ak_ex void
ak_str_remove(ak_str* s, uint32_t idx);
ak_ex void
ak_str_remove_range(ak_str* s,
                    uint32_t idx,
                    uint32_t count);

ak_ex void
ak_str_cstr_insert(ak_str* s,
                   uint32_t idx,
                   const char* cstr);
ak_ex void
ak_str_cstr_overwrite(ak_str* s,
                      uint32_t idx,
                      const char* cstr);
ak_ex void
ak_str_cstr_pushback(ak_str* s,
                     const char* cstr);

ak_ex void
ak_str_str_pushback(ak_str* dest,
                    const ak_str* src);

#endif
