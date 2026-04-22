#ifndef ak_coll_sibt_h
#define ak_coll_sibt_h

#include "ak/coll/sla.h"
#include "ak/export.h"
#include <stdbool.h>
#include <stdint.h>

typedef uint32_t ak_sibt_h;
typedef struct ak_sibt ak_sibt;
struct ak_sibt
{
  ak_sla slots;
  ak_sibt_h root;
  uint32_t itemsize;
};

ak_ex ak_sibt
ak_sibt_make(uint32_t itemsize,
             ak_alct alct);
ak_ex void
ak_sibt_destroy(ak_sibt* t);

ak_ex ak_sibt_h
ak_sibt_n_parent(ak_sibt* t, ak_sibt_h h);
ak_ex ak_sibt_h
ak_sibt_n_firstchild(ak_sibt* t,
                     ak_sibt_h h);
ak_ex ak_sibt_h
ak_sibt_n_nextsib(ak_sibt* t, ak_sibt_h h);

ak_ex ak_sibt_h
ak_sibt_n_add(ak_sibt* t,
              ak_sibt_h p_h,
              const void* item);
ak_ex void
ak_sibt_n_remove(ak_sibt* t, ak_sibt_h h);

#endif
