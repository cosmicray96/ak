
#include "ak/core/io.h"
#include "ak/debug.h"

#include <stdlib.h>

#include <glad/glad.h>

//--- private ---//
/*
 */

//===== program =====//
//--- private ---//
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

//--- public ---//
GLuint
program_make(const char* vs_src,
             const char* fs_src)
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
