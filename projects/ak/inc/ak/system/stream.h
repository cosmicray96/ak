#ifndef ak_system_stream_h
#define ak_system_stream_h

#include "ak/coll/str.h"
#include "ak/core/mem/allocator.h"
#include "ak/export.h"
#include <stdbool.h>
#include <stdint.h>

typedef enum
{
  ak_stmerr_ok = 0,
  ak_stmerr_err,
  ak_stmerr_end,
  ak_stmerr_invalid,
  ak_stmerr_notfound,
  ak_stmerr_unsupported,

} ak_stmerr;

typedef enum
{
  ak_stmtype_none,
  ak_stmtype_file,
  ak_stmtype_sio,
  ak_stmtype_ast
} ak_stmtype;

typedef struct
{
  ak_stmerr err;
  uint64_t transferred;
} ak_stmresult;

typedef struct
{
  ak_stmtype type;
  void* ctx;
} ak_stm;

ak_ex ak_stmerr
ak_stm_open_file(const char* path,
                 const char* mode,
                 ak_stm* o_stm);

ak_ex ak_stmerr
ak_stm_open_ast(ak_stm* o_stm);

ak_ex ak_stmerr
ak_stm_close(ak_stm stm);

ak_ex ak_stmresult
ak_stm_write(ak_stm stm,
             const void* data,
             uint64_t size);

ak_ex ak_stmresult
ak_stm_read(ak_stm stm,
            void* data,
            uint64_t size);

ak_ex ak_stmresult
ak_stm_read_all(ak_stm stm,
                void** o_data,
                uint64_t* o_size,
                ak_alct alct);

ak_ex ak_stmerr
ak_stm_read_line(ak_stm stm, ak_str* o_str);

#define ak_stm_try(x)                       \
  do {                                      \
    ak_stmerr ak_stmerr_var = (x);          \
    if (ak_stmerr_var) {                    \
      return ak_stmerr_var;                 \
    }                                       \
  } while (0)

#define ak_stm_try1(x)                      \
  do {                                      \
    ak_stmerr_var = (x);                    \
    if (ak_stmerr_var) {                    \
      return ak_stmerr_var;                 \
    }                                       \
  } while (0)
#define ak_stm_try2(x)                      \
  do {                                      \
    ak_stmerr_var = (x);                    \
    if (ak_stmerr_var != ak_stmerr_ok)      \
      goto ak_stm_crash;                    \
  } while (0)

ak_ex ak_stmerr
ak_stm_write_char(ak_stm stm, char value);
ak_ex ak_stmerr
ak_stm_read_char(ak_stm stm, char* o_value);

ak_ex ak_stmerr
ak_stm_write_bool(ak_stm stm, bool value);
ak_ex ak_stmerr
ak_stm_read_bool(ak_stm stm, bool* o_value);

//--- Unsigned Ints ---//
ak_ex ak_stmerr
ak_stm_write_u8(ak_stm stm, uint8_t value);
ak_ex ak_stmerr
ak_stm_read_u8(ak_stm stm, uint8_t* o_value);

ak_ex ak_stmerr
ak_stm_write_u16(ak_stm stm, uint16_t value);
ak_ex ak_stmerr
ak_stm_read_u16(ak_stm stm,
                uint16_t* o_value);

ak_ex ak_stmerr
ak_stm_write_u32(ak_stm stm, uint32_t value);
ak_ex ak_stmerr
ak_stm_read_u32(ak_stm stm,
                uint32_t* o_value);

ak_ex ak_stmerr
ak_stm_write_u64(ak_stm stm, uint64_t value);
ak_ex ak_stmerr
ak_stm_read_u64(ak_stm stm,
                uint64_t* o_value);

//--- Signed Ints ---//
ak_ex ak_stmerr
ak_stm_write_i8(ak_stm stm, int8_t value);
ak_ex ak_stmerr
ak_stm_read_i8(ak_stm stm, int8_t* o_value);

ak_ex ak_stmerr
ak_stm_write_i16(ak_stm stm, int16_t value);
ak_ex ak_stmerr
ak_stm_read_i16(ak_stm stm,
                int16_t* o_value);

ak_ex ak_stmerr
ak_stm_write_i32(ak_stm stm, int32_t value);
ak_ex ak_stmerr
ak_stm_read_i32(ak_stm stm,
                int32_t* o_value);

ak_ex ak_stmerr
ak_stm_write_i64(ak_stm stm, int64_t value);
ak_ex ak_stmerr
ak_stm_read_i64(ak_stm stm,
                int64_t* o_value);

//--- Floats ---//
ak_ex ak_stmerr
ak_stm_write_f32(ak_stm stm, float value);
ak_ex ak_stmerr
ak_stm_read_f32(ak_stm stm, float* o_value);
ak_ex ak_stmerr
ak_stm_write_f64(ak_stm stm, double value);
ak_ex ak_stmerr
ak_stm_read_f64(ak_stm stm, double* o_value);

#endif
