#include "ak/gfx/tex.h"
#include "ak/core/mem/allocator.h"

#include <glad/glad.h>

//===== ak_tex =====//
//--- private ---//
struct ak_tex
{
  ak_alct alct;
  ak_gfx* gfx;
  ak_textype type;
  GLuint id;
};

//--- impl ---//
GLuint
ak_tex_get(ak_tex* tex)
{
  return tex->id;
}

//--- internal ---//
ak_tex*
ak_tex_make_from_img(ak_gfx* gfx,
                     ak_img* img,
                     ak_textype type,
                     ak_alct alct)
{
  ak_tex* tex =
    ak_alct_alloc(alct, sizeof(ak_tex));
  tex->alct = alct;
  tex->gfx = gfx;

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

  glBindTexture(GL_TEXTURE_2D, 0);

  return tex;
}

void
ak_tex_destroy(ak_tex* tex)
{
  ak_alct_free(tex->alct, tex);
}

ak_textype
ak_tex_type(ak_tex* tex)
{
  return tex->type;
}
