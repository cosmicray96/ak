#include "ak/gfx/reses/tex.h"

typedef struct
{
  ak_errcode err;
  ak_gfx* gfx;
  const ak_img* img;
  ak_textype type;
  ak_tex** o_tex;
  ak_alct alct;
} tex_make;
static void
tex_make_fn(void* ctx)
{
  tex_make* t = ctx;
  t->err = ak_tex_from_img(t->gfx,
                           t->img,
                           t->type,
                           t->o_tex,
                           t->alct);
}
ak_errcode
ak_tex_from_img_dispatch(ak_dispatcher* d,
                         ak_gfx* gfx,
                         const ak_img* img,
                         ak_textype type,
                         ak_tex** o_tex,
                         ak_alct alct)
{
  tex_make t = { .gfx = gfx,
                 .img = img,
                 .type = type,
                 .o_tex = o_tex,
                 .alct = alct };
  ak_dispatcher_run(d, &tex_make_fn, &t);
  return t.err;
}

static void
tex_destroy_fn(void* ctx)
{
  ak_tex* tex = ctx;
  ak_tex_destroy(tex);
}
void
ak_tex_destroy_dispatch(ak_dispatcher* d,
                        ak_tex* tex)
{
  ak_dispatcher_run(d, &tex_destroy_fn, tex);
}
