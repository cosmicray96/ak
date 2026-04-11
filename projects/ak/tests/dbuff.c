#include "ak/core/mem/allocator.h"
#include "ak/core/mem/heap.h"
#include "ak/debug.h"
#include <ak/coll/dbuff.h>
#include <stdio.h>
#include <string.h>

int
main()
{
  printf("dbuff...\n");

  ak_heap heap = ak_heap_make();
  ak_alct alct = ak_heap_to_alct(&heap);

  {
    printf("basic...");

    ak_dbuff d =
      ak_dbuff_make(1, 2, 10, alct);
    ak_dbuff_destroy(&d);
    printf("done.\n");
  }
  {
    printf("basic2...");

    ak_dbuff d =
      ak_dbuff_make(1, 1.5f, 10, alct);

    ak_assert(ak_dbuff_cap(&d) == 10);
    ak_dbuff_grow(&d);
    ak_assert(ak_dbuff_cap(&d) == 15);

    ak_dbuff_destroy(&d);

    printf("done.\n");
  }
  {
    printf("basic3...");

    ak_dbuff d =
      ak_dbuff_make(1, 2, 10, alct);

    ak_assert(ak_dbuff_cap(&d) == 10);
    ak_dbuff_grow(&d);
    ak_assert(ak_dbuff_cap(&d) == 20);
    ak_dbuff_grow(&d);
    ak_assert(ak_dbuff_cap(&d) == 40);
    ak_dbuff_grow(&d);
    ak_assert(ak_dbuff_cap(&d) == 80);

    ak_dbuff_destroy(&d);

    printf("done.\n");
  }
  {
    printf("basic4...");

    ak_dbuff d =
      ak_dbuff_make(1, 2, 10, alct);
    ak_assert(ak_dbuff_cap(&d) == 10);

    char* buf = ak_dbuff_ptr(&d);
    buf = "hello";

    ak_dbuff_grow_w_copy(&d);
    ak_assert(ak_dbuff_cap(&d) == 20);
    ak_assert(strcmp(buf, ak_dbuff_ptr(&d)));

    ak_dbuff_grow_w_copy(&d);
    ak_assert(ak_dbuff_cap(&d) == 40);
    ak_assert(strcmp(buf, ak_dbuff_ptr(&d)));

    ak_dbuff_grow_w_copy(&d);
    ak_assert(ak_dbuff_cap(&d) == 80);
    ak_assert(strcmp(buf, ak_dbuff_ptr(&d)));

    ak_dbuff_destroy(&d);

    printf("done.\n");
  }
  printf("dbuff done.\n");
  return 0;
}
