#include "ak/renderer/renderer.h"
#include "ak/core/io.h"
#include "ak/renderer/core.h"
#include <glad/glad.h>

#include <stdlib.h>

//--- private ---//
static const char* vs_src =
  "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;\n"
  "void main() {\n"
  "  gl_Position = vec4(aPos, 1.0);\n"
  "}\n";

static const char* fs_src =
  "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main() {\n"
  "  FragColor = vec4(1.0, 0.5, 0.2, 1.0);\n"
  "}\n";

static GLuint
compile_shader(GLenum type, const char* src)
{
  GLuint s = glCreateShader(type);
  glShaderSource(s, 1, &src, NULL);
  glCompileShader(s);

  GLint ok = 0;
  glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
  if (!ok) {
    GLint len = 0;
    glGetShaderiv(
      s, GL_INFO_LOG_LENGTH, &len);
    char* log = malloc(len);
    glGetShaderInfoLog(s, len, NULL, log);
    ak_iostream_write_str(ak_iostream_sio(),
                          log);
    free(log);
    exit(EXIT_FAILURE);
  }
  return s;
}

static GLuint
make_program(void)
{
  GLuint vs =
    compile_shader(GL_VERTEX_SHADER, vs_src);
  GLuint fs = compile_shader(
    GL_FRAGMENT_SHADER, fs_src);
  GLuint p = glCreateProgram();
  glAttachShader(p, vs);
  glAttachShader(p, fs);
  glLinkProgram(p);

  GLint ok = 0;
  glGetProgramiv(p, GL_LINK_STATUS, &ok);
  if (!ok) {
    GLint len = 0;
    glGetProgramiv(
      p, GL_INFO_LOG_LENGTH, &len);
    char* log = malloc(len);
    glGetProgramInfoLog(p, len, NULL, log);
    ak_iostream_write_str(ak_iostream_sio(),
                          log);
    free(log);
    exit(EXIT_FAILURE);
  }
  glDeleteShader(vs);
  glDeleteShader(fs);
  return p;
}

//===== ak_renderer =====//
//--- private ---//
struct ak_renderer
{
  ak_alct alct;
  GLuint VAO;
  GLuint program;
};

//--- public ---//
ak_renderer*
ak_renderer_startup(ak_alct alct)
{
  ak_renderer* r =
    ak_alct_alloc(alct, sizeof(ak_renderer));
  r->alct = alct;

  {
    // 6) Setup triangle data (VAO + VBO)
    float vertices[] = {
      // positions (x, y, z)
      0.0f, 0.5f, 0.0f,  -0.5f, -0.5f,
      0.0f, 0.5f, -0.5f, 0.0f
    };

    GLuint VBO;
    glGenVertexArrays(1, &r->VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(r->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(vertices),
                 vertices,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          3 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }
  {
    r->program = make_program();
  }

  return r;
}
void
ak_renderer_shutdown(ak_renderer* r)
{
  ak_alct_free(r->alct, r);
}

void
ak_renderer_resize(ak_renderer* r,
                   int32_t w,
                   int32_t h)
{
}

void
ak_renderer_render(ak_renderer* r)
{
  glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(r->program);
  glBindVertexArray(r->VAO);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}
