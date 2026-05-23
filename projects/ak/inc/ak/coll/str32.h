#ifndef ak_coll_str32_h
#define ak_coll_str32_h

#include "ak/coll/da.h"
#include "ak/core/mem/allocator.h"
#include "ak/export.h"
#include <stdint.h>

//===== ak_codepoint =====//
typedef uint32_t ak_codepoint;

int32_t
ak_codepoint_from_utf8(const uint8_t* utf,
                       ak_codepoint* o_cp);
int32_t
ak_utf8_from_codepoint(ak_codepoint cp,
                       uint8_t* o_utf8);
bool
ak_codepoint_valid(ak_codepoint cp);

//===== ak_str32 =====//
typedef struct
{
  ak_da s;
} ak_str32;
ak_ex ak_str32
ak_str32_make(ak_alct alct);
ak_ex bool
ak_str32_make_from_cstr(const char* cstr,
                        ak_alct alct,
                        ak_str32* o_s);
ak_ex void
ak_str32_destroy(ak_str32* s);

ak_ex uint32_t
ak_str32_count(const ak_str32* s);

ak_ex ak_codepoint
ak_str32_at(const ak_str32* s, uint32_t idx);

ak_ex void
ak_str32_pushback(ak_str32* s,
                  ak_codepoint cp);
ak_ex void
ak_str32_insert(ak_str32* s,
                uint32_t idx,
                ak_codepoint cp);
ak_ex void
ak_str32_overwrite(ak_str32* s,
                   uint32_t idx,
                   ak_codepoint cp);
ak_ex void
ak_str32_remove(ak_str32* s, uint32_t idx);
ak_ex void
ak_str32_remove_range(ak_str32* s,
                      uint32_t idx,
                      uint32_t count);

//===== ak_str32v =====//
typedef struct
{
  ak_str32* s;
  uint32_t idx;
  uint32_t count;
} ak_str32v;

ak_str32v
ak_str32v_make(ak_str32* s,
               uint32_t idx,
               uint32_t count);
ak_str32v
ak_str32v_make_full(ak_str32* s);

ak_ex void
ak_str32v_pushback(ak_str32* s,
                   ak_str32v sv);
ak_ex void
ak_str32v_insert(ak_str32* s,
                 uint32_t idx,
                 ak_str32v sv);
ak_ex void
ak_str32v_overwrite(ak_str32* s,
                    uint32_t idx,
                    ak_str32v sv);

#endif
