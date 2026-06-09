#include "ak/system/stream.h"

ak_ex ak_stmerr
ak_stm_file_close(void* file);
ak_ex ak_stmerr
ak_stm_file_write(void* file,
                  const void* data,
                  uint64_t size);
ak_ex ak_stmerr
ak_stm_file_read(void* file,
                 void* data,
                 uint64_t size);
ak_stmerr
ak_stm_file_read_all(void* file,
                     void** o_data,
                     uint64_t* o_size,
                     ak_alct alct);
