#include <ak/coll/spa.h>
#include <ak/core/mem/heap.h>
#include <ak/debug.h>
#include <ak/program/program.h>
#include <stdint.h>

int
main()
{
  ak_program_startup();

  ak_log("spa start");

  ak_heap heap = ak_heap_make();
  ak_alct alct = ak_heap_to_alct(&heap);

  {
    ak_log("basic");

    ak_spa s =
      ak_spa_make(sizeof(uint32_t), alct);
    ak_spa_destroy(&s);

    ak_log("done.");
  }
  {
    ak_log("basic2");

    ak_spa s =
      ak_spa_make(sizeof(uint32_t), alct);

    for (uint32_t i = 1; i < 10; i++) {
      uint32_t v = i * 100;
      ak_spa_insert(&s, i, &v);
    }
    for (uint32_t i = 1; i < 10; i++) {
      ak_assert(ak_spa_exist(&s, i));
      uint32_t* v = ak_spa_at(&s, i);
      ak_assert(*v == i * 100);
    }

    ak_spa_destroy(&s);

    ak_log("done.");
  }
  ak_log("spa done");
  ak_program_shutdown();
  return 0;
}
