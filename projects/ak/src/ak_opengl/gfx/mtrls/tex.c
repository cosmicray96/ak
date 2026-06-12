/*#include "ak/core/math/fixed.h"
#include "ak/core/math/mat3x3.h"
#include "ak/core/mem/allocator.h"
#include "ak/debug.h"
#include "ak/gfx/core.h"
#include "ak/gfx/gresman.h"
#include "ak/gfx/mtrl_itn.h"
#include "ak_opengl/gfx/convert.h"
#include "ak_opengl/gfx/gfx_impl.h"
#include "ak_opengl/gfx/tex_impl.h"

#include <stddef.h>

#include <glad/glad.h>

#define check_err                           \
  do {                                      \
    GLenum err = glGetError();              \
    if (err != GL_NO_ERROR) {               \
      ak_log("opengl error:");              \
      ak_logv(err, x);                      \
      ak_assert(false);                     \
    }                                       \
  } while (0);

//===== ak_mtrl_col =====//
//--- private ---//

#define s_str_(s) #s
#define s_str(s) s_str_(s)

#define s_vert                              \
  layout(location = 0) in vec2 vi_cor;      \
  layout(location = 1) in vec3 vi_m1;       \
  layout(location = 2) in vec3 vi_m2;       \
  layout(location = 3) in vec3 vi_m3;       \
  layout(location = 4) in vec4 vi_uv;       \
  uniform mat3 u_vp;                        \
  uniform float u_t;                        \
  out vec2 vo_uv;                           \
  void main()                               \
  {                                         \
    mat3 m = mat3(vi_m1, vi_m2, vi_m3);     \
    vec3 pos =                              \
      u_vp * m * vec3(vi_cor, 1.0);         \
    gl_Position = vec4(pos.xy, 0.0, 1.0);   \
    vo_uv = vec2(                           \
      mix(                                  \
        vi_uv.x, vi_uv.z, vi_cor.x + 0.5),  \
      mix(                                  \
        vi_uv.y, vi_uv.w, vi_cor.y + 0.5)); \
  }
//vo_uv.x += u_t;                         \

static const char* vs_src =
  "#version 330 core\n" s_str(s_vert);

#define s_frag                              \
  uniform sampler2D u_tex;                  \
  in vec2 vo_uv;                            \
  out vec4 fo_col;                          \
  void main()                               \
  {                                         \
    vec4 color = texture(u_tex, vo_uv);     \
    fo_col = vec4(vo_uv, 0.0, 1.0);         \
    fo_col = color;                         \
  }

static const char* fs_src =
  "#version 330 core\n" s_str(s_frag);

typedef struct
{
  ak_mat3_f m;
  ak_opengl_quad_uv uv;
} quad;

typedef struct ak_mtrl_tex ak_mtrl_tex;
struct ak_mtrl_tex
{
  ak_alct alct;
  ak_gfx* g;
  GLuint vao;
  GLuint program;
  GLuint vp_loc;
  GLuint tex_loc;
  GLuint t_loc;

  ak_gresid gid;

  bool call_begin;
};

//--- internal ---//
ak_mtrl_tex*
ak_mtrl_tex_make(ak_gfx* g, ak_alct alct)
{
  ak_mtrl_tex* m =
    ak_alct_alloc(alct, sizeof(ak_mtrl_tex));
  m->alct = alct;
  m->g = g;

  m->program = program_make(fs_src, vs_src);
  glUseProgram(m->program);
  m->vp_loc =
    glGetUniformLocation(m->program, "u_vp");
  m->tex_loc = glGetUniformLocation(
    m->program, "u_tex");
  m->t_loc =
    glGetUniformLocation(m->program, "u_t");

  // ak_assert(m->vp_loc != -1);
  glUseProgram(0);

  glGenVertexArrays(1, &m->vao);
  glBindVertexArray(m->vao);

  ak_gfx_buff_bind_ebo(g);

  ak_gfx_buff_bind_vbo(g);
  glVertexAttribPointer(0,
                        2,
                        GL_FLOAT,
                        GL_FALSE,
                        2 * sizeof(float),
                        (void*)0);
  glEnableVertexAttribArray(0);

  ak_gfx_buff_bind_ivbo(g);
  glVertexAttribPointer(
    1,
    3,
    GL_FLOAT,
    GL_FALSE,
    sizeof(quad),
    (void*)offsetof(quad, m.v[0]));
  glEnableVertexAttribArray(1);
  glVertexAttribDivisor(1, 1);

  glVertexAttribPointer(
    2,
    3,
    GL_FLOAT,
    GL_FALSE,
    sizeof(quad),
    (void*)offsetof(quad, m.v[3]));
  glEnableVertexAttribArray(2);
  glVertexAttribDivisor(2, 1);

  glVertexAttribPointer(
    3,
    3,
    GL_FLOAT,
    GL_FALSE,
    sizeof(quad),
    (void*)offsetof(quad, m.v[6]));
  glEnableVertexAttribArray(3);
  glVertexAttribDivisor(3, 1);

  glVertexAttribPointer(
    4,
    4,
    GL_FLOAT,
    GL_FALSE,
    sizeof(quad),
    (void*)offsetof(quad, uv));
  glEnableVertexAttribArray(4);
  glVertexAttribDivisor(4, 1);

  glBindVertexArray(0);

  ak_gfx_buff_unbind_vbo(g);
  ak_gfx_buff_unbind_ivbo(g);
  ak_gfx_buff_unbind_ebo(g);

  m->call_begin = false;

  return m;
}

void
ak_mtrl_tex_destroy(void* mtrl)
{
  ak_mtrl_tex* m = mtrl;
  glDeleteVertexArrays(1, &m->vao);
  glDeleteProgram(m->program);
  ak_alct_free(m->alct, m);
}

//--- export ---//
void
ak_mtrl_tex_call_begin(
  void* mtrl,
  ak_gresreg* grr,
  const ak_mtrl_basedata* bd,
  const ak_mtrl_indata* id)
{
  ak_mtrl_tex* m = mtrl;
  ak_assert(!m->call_begin);

  glUseProgram(m->program);
  glUniformMatrix3fv(
    m->vp_loc, 1, GL_FALSE, id->vp.v);
  glUniform1f(m->t_loc, id->time);

  ak_tex_old tex = bd->tex;

  ak_tex* t =
    ak_gresreg_get_tex(grr, tex.gid);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D,
                ak_tex_get(t));
  glUniform1i(m->tex_loc, 0);

  if (tex.filter_type == ak_filter_linear) {
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER,
                    GL_LINEAR);
  } else {
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER,
                    GL_NEAREST);
  }
  if (tex.uv_type == ak_uv_clamp) {
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S,
                    GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T,
                    GL_CLAMP_TO_EDGE);

  } else {
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S,
                    GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T,
                    GL_REPEAT);
  }

  glBindVertexArray(m->vao);

  ak_gfx_call_begin(m->g, sizeof(quad));

  m->call_begin = true;
}

void
ak_mtrl_tex_call_end(void* mtrl)
{
  ak_mtrl_tex* m = mtrl;
  ak_assert(m->call_begin);
  ak_gfx_call_end(m->g);
  glUseProgram(0);
  glBindVertexArray(0);

  m->call_begin = false;
}

void
ak_mtrl_tex_pushquad(
  void* mtrl,
  const ak_mtrl_quaddata* qd,
  const ak_mat3_f* gmat3f)
{
  ak_mtrl_tex* m = mtrl;
  ak_assert(m->call_begin);

  quad q = { .uv = ak_opengl_convert_uv(
               qd->uv_min, qd->uv_max),
             .m = *gmat3f };

  ak_gfx_pushquad(m->g, &q);
}
*/
