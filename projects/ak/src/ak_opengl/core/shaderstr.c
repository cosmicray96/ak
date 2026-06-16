#include "ak/core/shaderstr.h"
#include "ak/coll/str.h"
#include "ak/debug.h"
#include "ak/system/stream.h"
#include <stdio.h>
#include <string.h>

ak_stmerr
ak_stm_read_shaderstr(ak_stm stm,
                      ak_shaderstr* o_ss,
                      ak_alct alct)
{
  o_ss->vert = ak_str_make(alct);
  o_ss->frag = ak_str_make(alct);

  ak_str line = ak_str_make(alct);

  ak_str* outline = &o_ss->vert;
  ak_stmerr err = ak_stmerr_ok;
  while (true) {
    err = ak_stm_read_line(stm, &line);
    if (err != ak_stmerr_ok) {
      break;
    }

    if (strcmp("#vert\n",
               ak_str_ptr_const(&line)) ==
        0) {
      outline = &o_ss->vert;
    } else if (strcmp("#frag\n",
                      ak_str_ptr_const(
                        &line)) == 0) {
      outline = &o_ss->frag;
    } else {
      ak_str_str_pushback(outline, &line);
    }

    ak_str_clear(&line);
  }

  if (err != ak_stmerr_end) {
    ak_shaderstr_destroy(o_ss);
    ak_str_destroy(&line);
    return err;
  }

  ak_str_destroy(&line);
  return ak_stmerr_ok;
}
