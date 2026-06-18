#include "ak/coll/stm.h"
#include "ak/system/stream.h"

ak_stmerr
ak_stm_write_da(ak_stm stm, const ak_da* da)
{
  uint32_t count = ak_da_count(da);
  uint32_t is = ak_da_itemsize(da);
  const void* ptr = ak_da_ptr_const(da);

  ak_stm_try(ak_stm_write_u32(stm, is));
  ak_stm_try(ak_stm_write_u32(stm, count));

  ak_stmresult r =
    ak_stm_write(stm, ptr, count * is);
  return r.err;
}

ak_stmerr
ak_stm_read_da(ak_stm stm,
               ak_da* o_da,
               ak_alct alct)
{
  uint32_t is = 0;
  ak_stm_try(ak_stm_read_u32(stm, &is));
  uint32_t count = 0;
  ak_stm_try(ak_stm_read_u32(stm, &count));

  void* data =
    ak_alct_alloc(alct, is * count);
  ak_stmresult r =
    ak_stm_read(stm, data, is * count);
  if (r.err != ak_stmerr_ok) {
    ak_alct_free(alct, data);
    return r.err;
  }

  *o_da =
    ak_da_from_ptr(data, is, count, alct);

  return ak_stmerr_ok;
}
