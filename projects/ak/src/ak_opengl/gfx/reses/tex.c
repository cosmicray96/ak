#include "ak/gfx/reses/tex.h"
#include "ak/core/mem/allocator.h"
#include "ak/debug.h"
#include "ak/gfx/core.h"
#include "ak_opengl/gfx/rctx.h"
#include "ak_opengl/gfx/reses/tex_impl.h"

//===== ak_tex =====//
//--- private ---//
struct ak_tex
{
  ak_alct alct;
  ak_rctx* rctx;
  ak_textype type;
  GLuint id;
};
ak_errcode
tex_from_img(ak_rctx* rctx,
             const ak_img* img,
             ak_textype type,
             ak_tex** o_tex,
             ak_alct alct)
{
  ak_tex* tex =
    ak_alct_alloc(alct, sizeof(ak_tex));
  tex->alct = alct;
  tex->rctx = rctx;

  glGenTextures(1, &tex->id);
  glBindTexture(GL_TEXTURE_2D, tex->id);

  glTexImage2D(GL_TEXTURE_2D,
               0,
               GL_RGBA8,
               img->w,
               img->h,
               0,
               GL_RGBA,
               GL_UNSIGNED_BYTE,
               img->pixels);

  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,
                  GL_TEXTURE_MAG_FILTER,
                  GL_LINEAR);

  GLenum err = glGetError();
  if (err != GL_NO_ERROR) {
    ak_log("Opengl. glerror: %d", err);
    return ak_err_invalid_args;
  }

  glBindTexture(GL_TEXTURE_2D, 0);

  *o_tex = tex;
  return ak_ok;
}

void
tex_destroy(ak_tex* tex)
{
  glDeleteTextures(1, &tex->id);
  ak_alct_free(tex->alct, tex);
}

//--- impl ---//
GLuint
ak_tex_get(ak_tex* tex)
{
  return tex->id;
}

//--- internal ---//
typedef struct
{
  ak_errcode err;
  ak_rctx* rctx;
  const ak_img* img;
  ak_textype type;
  ak_tex** o_tex;
  ak_alct alct;
} tex_make;
static void
tex_make_fn(void* ctx)
{
  tex_make* t = ctx;
  t->err = tex_from_img(t->rctx,
                        t->img,
                        t->type,
                        t->o_tex,
                        t->alct);
}
ak_errcode
ak_tex_from_img(ak_rctx* rctx,
                const ak_img* img,
                ak_textype type,
                ak_tex** o_tex,
                ak_alct alct)
{
  tex_make t = { .rctx = rctx,
                 .img = img,
                 .type = type,
                 .o_tex = o_tex,
                 .alct = alct };
  ak_dispatcher_wake_run(
    ak_opengl_rctx_dispatcher(rctx),
    &tex_make_fn,
    &t);
  return t.err;
}

static void
tex_destroy_fn(void* ctx)
{
  ak_tex* tex = ctx;
  tex_destroy(tex);
}
void
ak_tex_destroy(ak_tex* tex)
{
  ak_dispatcher_wake_run(
    ak_opengl_rctx_dispatcher(tex->rctx),
    &tex_destroy_fn,
    tex);
}

ak_textype
ak_tex_type(ak_tex* tex)
{
  return tex->type;
}
