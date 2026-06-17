
#include "ak/core/errcode.h"
#include "ak/core/io.h"
#include "ak/debug.h"

#include <stdlib.h>

#include <glad/glad.h>

//--- private ---//
/*
 */

//===== program =====//
//--- private ---//

static ak_errcode
compile_shader_with_err(GLenum type,
                        const char* src,
                        GLuint* o_shader)
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
    ak_log("Opengl. Shader Comp Error: %s",
           log);
    free(log);
    return ak_err_invalid_args;
  }
  *o_shader = s;
  return ak_ok;
}

//--- public ---//

ak_errcode
program_make_with_err(const char* vs_src,
                      const char* fs_src,
                      GLuint* o_program)
{
  GLuint vs = 0;
  ak_err_try(compile_shader_with_err(
    GL_VERTEX_SHADER, vs_src, &vs));

  GLuint fs = 0;
  ak_err_try(compile_shader_with_err(
    GL_FRAGMENT_SHADER, fs_src, &fs));

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
    ak_log("Opengl. Shader Comp Error: %s",
           log);
    free(log);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return ak_err_invalid_args;
  }
  glDeleteShader(vs);
  glDeleteShader(fs);

  *o_program = p;
  return ak_ok;
}
