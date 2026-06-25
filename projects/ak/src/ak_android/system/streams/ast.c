#include "ak/system/stream_itn.h"

ak_ex ak_stmerr
ak_stm_open_ast(ak_stm* o_stm)
{
  return ak_stmerr_err;
}

ak_ex ak_stmerr
ak_stm_ast_close(void* ast)
{

  return ak_stmerr_err;
}

ak_stmresult
ak_stm_ast_write(void* ast,
                 const void* data,
                 uint64_t size)
{

  return (ak_stmresult){ .err =
                           ak_stmerr_err };
}

ak_stmresult
ak_stm_ast_read(void* ast,
                void* data,
                uint64_t size)
{

  return (ak_stmresult){ .err =
                           ak_stmerr_err };
}
ak_stmresult
ak_stm_ast_read_all(void* ast,
                    void** o_data,
                    uint64_t* o_size,
                    ak_alct alct)
{

  return (ak_stmresult){ .err =
                           ak_stmerr_err };
}
