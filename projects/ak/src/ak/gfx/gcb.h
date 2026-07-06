#ifndef ak_gfx_gcb_h
#define ak_gfx_gcb_h

#include "ak/coll/da.h"
#include "ak/core/math/mat3x3.h"
#include "ak/core/mem/allocator.h"
#include "ak/gfx/core.h"

typedef enum
{
  ak_gcbcmdtype_quad,
  ka_gcbcmdtype_call,
  ak_gcbcmdtype_batch,
  ak_gcbcmdtype_scissor,
  ak_gcbcmdtype_scissor_reset,
  ak_gcbcmdtype_resize,
} ak_gcbcmdtype;
typedef struct
{
  ak_gcbcmdtype type;
  union
  {
    struct
    {
      ak_gfx_batchdata bd;
    } batch;
    struct
    {
      ak_gfx_calldata cd;
    } call;
    struct
    {
      ak_gfx_quaddata qd;
      ak_mat3_f gmat;
    } quad;
    struct
    {
      int32_t x;
      int32_t y;
      uint32_t w;
      uint32_t h;
    } scissor;
    struct
    {
      uint32_t w_screen;
      uint32_t h_screen;
      uint32_t x;
      uint32_t y;
      uint32_t w;
      uint32_t h;
    } resize;
  };
} ak_gcbcmd;

typedef struct
{
  ak_da cmds;
} ak_gcb;

ak_gcb
ak_gcb_make(ak_alct alct);
void
ak_gcb_destroy(ak_gcb* gcb);
void
ak_gcb_clear(ak_gcb* gcb);

void
ak_gcb_joinback(ak_gcb* dest, ak_gcb* src);

void
ak_gcb_push(ak_gcb* gcb,
            const ak_gcbcmd* cmd);

void
ak_gcb_push_batch(
  ak_gcb* gcb,
  const ak_gfx_batchdata* bd);
void
ak_gcb_push_call(ak_gcb* gcb,
                 const ak_gfx_calldata* cd);
void
ak_gcb_push_quad(ak_gcb* gcb,
                 const ak_gfx_quaddata* qd,
                 const ak_mat3_f* gmat);

void
ak_gcb_push_scissor_reset(ak_gcb* gcb);
void
ak_gcb_push_scissor(ak_gcb* gcb,
                    int32_t x,
                    int32_t y,
                    uint32_t w,
                    uint32_t h);

void
ak_gcb_push_resize(ak_gcb* gcb,
                   uint32_t w,
                   uint32_t h);

#endif
