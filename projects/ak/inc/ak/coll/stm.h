#ifndef ak_coll_stm_h
#define ak_coll_stm_h

#include "ak/core/mem/allocator.h"
#include "ak/export.h"
#include "ak/system/stream.h"

ak_ex ak_stmerr
ak_stm_write_da(ak_stm stm, const ak_da* da);
ak_ex ak_stmerr
ak_stm_read_da(ak_stm stm,
               ak_da* o_da,
               ak_alct alct);

#endif
