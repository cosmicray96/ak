#include <ak/coll/sla.h>
#include <ak/core/mem/heap.h>
#include <ak/debug.h>
#include <ak/program/program.h>
#include <stdint.h>

int
main()
{
  ak_program_startup();

  ak_log("sla start");

  ak_heap heap = ak_heap_make();
  ak_alct alct = ak_heap_to_alct(&heap);

  {
    ak_log("basic");

    ak_sla s =
      ak_sla_make(sizeof(uint32_t), alct);
    ak_sla_destroy(&s);

    ak_log("done.");
  }
  {
    ak_log("basic2");

    ak_sla s =
      ak_sla_make(sizeof(uint32_t), alct);
    ak_sla_destroy(&s);
    ak_sla_h hs[20] = { 0 };

    for (uint32_t i = 0; i < 20; i++) {
      uint32_t v = i * 100;
      hs[i] = ak_sla_insert(&s, &v);
    }
    for (uint32_t i = 0; i < 20; i++) {
      ak_assert(ak_sla_exist(&s, hs[i]));
      uint32_t* v = ak_sla_at(&s, hs[i]);
      ak_assert(*v == i * 100);
    }

    ak_log("done.");
  }
  ak_log("sla done");
  ak_program_shutdown();
  return 0;
}
