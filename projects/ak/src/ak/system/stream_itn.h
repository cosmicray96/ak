#include "ak/system/stream.h"

ak_ex ak_errcode
ak_stm_file_close(void* file);
ak_ex ak_stmresult
ak_stm_file_write(void* file,
                  const void* data,
                  uint64_t size);
ak_ex ak_stmresult
ak_stm_file_read(void* file,
                 void* data,
                 uint64_t size);
ak_stmresult
ak_stm_file_read_all(void* file,
                     void** o_data,
                     uint64_t* o_size,
                     ak_alct alct);

ak_ex ak_errcode
ak_stm_ast_close(void* ast);
ak_ex ak_stmresult
ak_stm_ast_write(void* ast,
                 const void* data,
                 uint64_t size);
ak_ex ak_stmresult
ak_stm_ast_read(void* ast,
                void* data,
                uint64_t size);
ak_stmresult
ak_stm_ast_read_all(void* ast,
                    void** o_data,
                    uint64_t* o_size,
                    ak_alct alct);
