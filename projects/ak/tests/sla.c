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
    uint32_t hs[20] = { 0 };

    for (uint32_t i = 0; i < 20; i++) {
      uint32_t v = i * 100;
      hs[i] = ak_sla_insert(&s, &v);
    }
    for (uint32_t i = 0; i < 20; i++) {
      uint32_t* v = ak_sla_at(&s, hs[i]);
      ak_assert(*v == i * 100);
    }

    ak_sla_destroy(&s);
    ak_log("done.");
  }
  {
    ak_log("basic3");

    ak_sla s =
      ak_sla_make(sizeof(uint32_t), alct);
    uint32_t hs[20] = { 0 };

    for (uint32_t i = 0; i < 20; i++) {
      uint32_t v = i * 100;
      hs[i] = ak_sla_insert(&s, &v);
    }
    ak_sla_remove(&s, hs[10]);
    for (uint32_t i = 0; i < 10; i++) {
      uint32_t* v = ak_sla_at(&s, hs[i]);
      ak_assert(*v == i * 100);
    }

    for (uint32_t i = 11; i < 20; i++) {
      uint32_t* v = ak_sla_at(&s, hs[i]);
      ak_assert(*v == i * 100);
    }

    ak_sla_destroy(&s);
    ak_log("done.");
  }
  {
    ak_log("basic4");

    ak_sla s =
      ak_sla_make(sizeof(uint32_t), alct);
    uint32_t hs[20] = { 0 };

    for (uint32_t i = 0; i < 20; i++) {
      uint32_t v = i * 100;
      hs[i] = ak_sla_insert(&s, &v);
    }
    for (uint32_t i = 5; i < 15; i++) {
      ak_sla_remove(&s, hs[i]);
    }

    for (uint32_t i = 0; i < 5; i++) {
      uint32_t* v = ak_sla_at(&s, hs[i]);
      ak_assert(*v == i * 100);
    }
    for (uint32_t i = 5; i < 15; i++) {
    }
    for (uint32_t i = 15; i < 20; i++) {
      uint32_t* v = ak_sla_at(&s, hs[i]);
      ak_assert(*v == i * 100);
    }

    for (uint32_t i = 5; i < 15; i++) {
      uint32_t v = i * 1000;
      hs[i] = ak_sla_insert(&s, &v);
    }

    for (uint32_t i = 0; i < 5; i++) {
      uint32_t* v = ak_sla_at(&s, hs[i]);
      ak_assert(*v == i * 100);
    }
    for (uint32_t i = 5; i < 15; i++) {
      uint32_t* v = ak_sla_at(&s, hs[i]);
      ak_assert(*v == i * 1000);
    }
    for (uint32_t i = 15; i < 20; i++) {
      uint32_t* v = ak_sla_at(&s, hs[i]);
      ak_assert(*v == i * 100);
    }

    ak_sla_destroy(&s);
    ak_log("done.");
  }
  ak_log("sla done");
  ak_program_shutdown();
  return 0;
}
