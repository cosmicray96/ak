#include "ak/system/stream.h"
#include "ak/system/stream_itn.h"
#include <stdio.h>

ak_ex ak_errcode
ak_stm_open_ast(const char* path,
                ak_stm* o_stm)
{
  char p[1024];
  int n = snprintf(
    p, sizeof(p), "assets/%s", path);
  if (n < 0 || (size_t)n >= sizeof(p)) {
    return ak_err_invalid_args;
  }
  return ak_stm_open_file(p, "rb", o_stm);
}

ak_errcode
ak_stm_ast_close(void* ast)
{
  return ak_stm_file_close(ast);
}

ak_stmresult
ak_stm_ast_write(void* ast,
                 const void* data,
                 uint64_t size)
{
  return ak_stm_file_write(ast, data, size);
}

ak_stmresult
ak_stm_ast_read(void* ast,
                void* data,
                uint64_t size)
{
  return ak_stm_file_read(ast, data, size);
}
ak_stmresult
ak_stm_ast_read_all(void* ast,
                    void** o_data,
                    uint64_t* o_size,
                    ak_alct alct)
{
  return ak_stm_file_read_all(
    ast, o_data, o_size, alct);
}
