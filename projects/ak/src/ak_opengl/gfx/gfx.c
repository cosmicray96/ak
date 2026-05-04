#include "ak/gfx/gfx.h"
#include "ak/core/io.h"
#include "ak/debug.h"
#include "ak/gfx/core.h"

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
  "uniform vec4 ourColor;"
  "out vec4 FragColor;\n"
  "void main() {\n"
  "  FragColor = ourColor;\n"
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
    ak_assert(false);
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
    ak_assert(false);
  }
  glDeleteShader(vs);
  glDeleteShader(fs);
  return p;
}

//===== ak_gfx =====//
//--- private ---//
struct ak_gfx
{
  ak_alct alct;
  GLuint VAO;
  GLuint VBO;
  GLuint EBO;

  GLuint program;

  float col;
};

//--- public ---//
ak_gfx*
ak_gfx_startup(ak_alct alct)
{
  ak_gfx* r =
    ak_alct_alloc(alct, sizeof(ak_gfx));
  r->alct = alct;

  {
    float vertices[] = {
      0.5f,  0.5f,  0.0f, // top right
      0.5f,  -0.5f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, // bottom left
      -0.5f, 0.5f,  0.0f  // top left
    };
    unsigned int indices[] = {
      // note that we start from 0!
      0, 1, 3, // first Triangle
      1, 2, 3  // second Triangle
    };

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    r->VAO = VAO;
    r->VBO = VBO;
    r->EBO = EBO;

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(vertices),
                 vertices,
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                 EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(indices),
                 indices,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          3 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
  {
    r->program = make_program();
  }
  // glPolygonMode(GL_FRONT_AND_BACK,
  // GL_LINE);

  return r;
}
void
ak_gfx_shutdown(ak_gfx* r)
{
  glDeleteVertexArrays(1, &r->VAO);
  glDeleteBuffers(1, &r->EBO);
  glDeleteBuffers(1, &r->VBO);

  glDeleteProgram(r->program);

  ak_alct_free(r->alct, r);
}
void
ak_gfx_col_set(ak_gfx* r, float col)
{
  r->col = col;
}

void
ak_gfx_resize(ak_gfx* r,
              uint32_t w,
              uint32_t h)
{
  glViewport(0, 0, w, h);
}

void
ak_gfx_flush(ak_gfx* r)
{
  glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(r->program);

  {
    int vertexColorLocation =
      glGetUniformLocation(r->program,
                           "ourColor");
    glUniform4f(vertexColorLocation,
                0.0f,
                r->col,
                0.0f,
                1.0f);
  }

  glBindVertexArray(r->VAO);
  glDrawElements(
    GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
  glUseProgram(0);
}
