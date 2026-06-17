#include "ak/system/stream.h"
#include "ak/core/mem/ptr.h"
#include "ak/system/stream_itn.h"
#include <stdio.h>

ak_stmerr
ak_stm_close(ak_stm stm)
{
  switch (stm.type) {
    case ak_stmtype_file: {
      return ak_stm_file_close(stm.ctx);
    }
    default: {
      return ak_stmerr_unsupported;
    }
  }
}
void
print_hex(const uint8_t* data, size_t len)
{
  for (size_t i = 0; i < len; i++) {
    printf("%02X ",
           data[i]); // 2-digit uppercase hex
  }
  printf("\n");
}

ak_stmresult
ak_stm_write(ak_stm stm,
             const void* data,
             uint64_t size)
{
  switch (stm.type) {
    case ak_stmtype_file: {
      return ak_stm_file_write(
        stm.ctx, data, size);
    }
    default: {
      return (ak_stmresult){
        .err = ak_stmerr_unsupported
      };
    }
  }
}

ak_stmresult
ak_stm_read(ak_stm stm,
            void* data,
            uint64_t size)
{
  ak_stmresult err = { .err =
                         ak_stmerr_err };
  switch (stm.type) {
    case ak_stmtype_file: {
      err = ak_stm_file_read(
        stm.ctx, data, size);
      break;
    }
    default: {
      return (ak_stmresult){
        .err = ak_stmerr_unsupported
      };
    }
  }
  return err;
}

ak_stmresult
ak_stm_read_all(ak_stm stm,
                void** o_data,
                uint64_t* o_size,
                ak_alct alct)
{
  ak_stmresult err = { .err =
                         ak_stmerr_err };
  switch (stm.type) {
    case ak_stmtype_file: {
      err = ak_stm_file_read_all(
        stm.ctx, o_data, o_size, alct);
      break;
    }
    default: {
      return (ak_stmresult){
        .err = ak_stmerr_unsupported
      };
    }
  }
  return err;
}

ak_stmerr
ak_stm_read_line(ak_stm stm, ak_str* o_str)
{
  char ch = 0;
  ak_stmerr err = ak_stmerr_err;

  while (true) {
    err = ak_stm_read_char(stm, &ch);

    if (err != ak_stmerr_ok) {
      break;
    }

    ak_str_pushback(o_str, ch);

    if (ch == '\n') {
      return ak_stmerr_ok;
    }
  }
  if (err == ak_stmerr_end &&
      ak_str_count(o_str) > 0) {
    return ak_stmerr_ok;
  }

  return err;
}

//--- char ---//
ak_stmerr
ak_stm_write_char(ak_stm stm, char value)
{
  return ak_stm_write(stm, &value, 1).err;
}
ak_stmerr
ak_stm_read_char(ak_stm stm, char* o_value)
{
  return ak_stm_read(stm, o_value, 1).err;
}

//--- bool ---//
ak_stmerr
ak_stm_write_bool(ak_stm stm, bool value)
{
  return ak_stm_write_u8(stm,
                         (uint8_t)value);
}
ak_stmerr
ak_stm_read_bool(ak_stm stm, bool* o_value)
{
  return ak_stm_read_u8(stm,
                        (uint8_t*)o_value);
}

//--- unsigned int ---//
ak_stmerr
ak_stm_write_u8(ak_stm stm, uint8_t value)
{
  return ak_stm_write(stm, &value, 1).err;
}
ak_stmerr
ak_stm_read_u8(ak_stm stm, uint8_t* o_value)
{
  return ak_stm_read(stm, o_value, 1).err;
}

ak_stmerr
ak_stm_write_u16(ak_stm stm, uint16_t value)
{
  uint8_t buf[2] = {
    (uint8_t)(value),
    (uint8_t)(value >> 8),
  };
  return ak_stm_write(stm, buf, 2).err;
}
ak_stmerr
ak_stm_read_u16(ak_stm stm,
                uint16_t* o_value)
{
  uint8_t buf[2];
  ak_stmerr err =
    ak_stm_read(stm, buf, 2).err;
  if (err)
    return err;
  *o_value = (uint16_t)(buf[0]) |
             (uint16_t)(buf[1]) << 8;
  return ak_stmerr_ok;
}

ak_stmerr
ak_stm_write_u32(ak_stm stm, uint32_t value)
{
  uint8_t buf[4] = {
    (uint8_t)(value),
    (uint8_t)(value >> 8),
    (uint8_t)(value >> 16),
    (uint8_t)(value >> 24),
  };
  return ak_stm_write(stm, buf, 4).err;
}
ak_stmerr
ak_stm_read_u32(ak_stm stm,
                uint32_t* o_value)
{
  uint8_t buf[4];
  ak_stmerr err =
    ak_stm_read(stm, buf, 4).err;
  if (err)
    return err;
  *o_value = (uint32_t)(buf[0]) |
             (uint32_t)(buf[1]) << 8 |
             (uint32_t)(buf[2]) << 16 |
             (uint32_t)(buf[3]) << 24;
  return ak_stmerr_ok;
}

ak_stmerr
ak_stm_write_u64(ak_stm stm, uint64_t value)
{
  uint8_t buf[8] = {
    (uint8_t)(value),
    (uint8_t)(value >> 8),
    (uint8_t)(value >> 16),
    (uint8_t)(value >> 24),
    (uint8_t)(value >> 32),
    (uint8_t)(value >> 40),
    (uint8_t)(value >> 48),
    (uint8_t)(value >> 56),
  };
  return ak_stm_write(stm, buf, 8).err;
}
ak_stmerr
ak_stm_read_u64(ak_stm stm,
                uint64_t* o_value)
{
  uint8_t buf[8];
  ak_stmerr err =
    ak_stm_read(stm, buf, 8).err;
  if (err)
    return err;
  *o_value = (uint64_t)(buf[0]) |
             (uint64_t)(buf[1]) << 8 |
             (uint64_t)(buf[2]) << 16 |
             (uint64_t)(buf[3]) << 24 |
             (uint64_t)(buf[4]) << 32 |
             (uint64_t)(buf[5]) << 40 |
             (uint64_t)(buf[6]) << 48 |
             (uint64_t)(buf[7]) << 56;
  return ak_stmerr_ok;
}

//--- signed int ---//
ak_stmerr
ak_stm_write_i8(ak_stm stm, int8_t value)
{
  return ak_stm_write_u8(stm,
                         (uint8_t)value);
}
ak_stmerr
ak_stm_read_i8(ak_stm stm, int8_t* o_value)
{
  return ak_stm_read_u8(stm,
                        (uint8_t*)o_value);
}

ak_stmerr
ak_stm_write_i16(ak_stm stm, int16_t value)
{
  return ak_stm_write_u16(stm,
                          (uint16_t)value);
}
ak_stmerr
ak_stm_read_i16(ak_stm stm, int16_t* o_value)
{
  return ak_stm_read_u16(stm,
                         (uint16_t*)o_value);
}

ak_stmerr
ak_stm_write_i32(ak_stm stm, int32_t value)
{
  return ak_stm_write_u32(stm,
                          (uint32_t)value);
}
ak_stmerr
ak_stm_read_i32(ak_stm stm, int32_t* o_value)
{
  return ak_stm_read_u32(stm,
                         (uint32_t*)o_value);
}

ak_stmerr
ak_stm_write_i64(ak_stm stm, int64_t value)
{
  return ak_stm_write_u64(stm,
                          (uint64_t)value);
}
ak_stmerr
ak_stm_read_i64(ak_stm stm, int64_t* o_value)
{
  return ak_stm_read_u64(stm,
                         (uint64_t*)o_value);
}

//--- floats ---//
ak_stmerr
ak_stm_write_f32(ak_stm stm, float value)
{
  uint32_t bits;
  ak_p_cpy(&bits, &value, 4);
  return ak_stm_write_u32(stm, bits);
}
ak_stmerr
ak_stm_read_f32(ak_stm stm, float* o_value)
{
  uint32_t bits;
  ak_stmerr err =
    ak_stm_read_u32(stm, &bits);
  if (err)
    return err;
  ak_p_cpy(o_value, &bits, 4);
  return ak_stmerr_ok;
}

ak_stmerr
ak_stm_write_f64(ak_stm stm, double value)
{
  uint64_t bits;
  ak_p_cpy(&bits, &value, 8);
  return ak_stm_write_u64(stm, bits);
}
ak_stmerr
ak_stm_read_f64(ak_stm stm, double* o_value)
{
  uint64_t bits;
  ak_stmerr err =
    ak_stm_read_u64(stm, &bits);
  if (err)
    return err;
  ak_p_cpy(o_value, &bits, 8);
  return ak_stmerr_ok;
}
