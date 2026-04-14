#include <ak/coll/hmn.h>
#include <ak/core/mem/heap.h>
#include <ak/debug.h>
#include <ak/program/program.h>
#include <stdint.h>

int
main()
{
  ak_program_startup();

  ak_log("hmn start");

  ak_heap heap = ak_heap_make();
  ak_alct alct = ak_heap_to_alct(&heap);

  {
    ak_log("basic");

    ak_hmn h =
      ak_hmn_make(sizeof(uint32_t), alct);

    ak_hmn_destroy(&h);

    ak_log("done.");
  }

  {
    ak_log("basic2");

    ak_hmn h =
      ak_hmn_make(sizeof(uint32_t), alct);

    uint32_t a = 50;

    ak_hmn_insert_u64(&h, 1, &a);
    uint32_t* ap = ak_hmn_at_u64(&h, 1);
    ak_assert(ap);
    ak_assert(*ap == a);

    ak_hmn_destroy(&h);

    ak_log("done.");
  }

  {
    ak_log("basic3");

    ak_hmn h =
      ak_hmn_make(sizeof(uint32_t), alct);

    for (uint32_t i = 0; i < 5; i++) {
      uint32_t v = i * 100;
      ak_hmn_insert_u64(&h, i, &v);
    }
    for (uint32_t i = 0; i < 5; i++) {
      uint32_t* v = ak_hmn_at_u64(&h, i);
      ak_assert(v);
      ak_assert(*v == i * 100);
    }

    ak_hmn_destroy(&h);

    ak_log("done.");
  }
  {
    ak_log("basic3");

    ak_hmn h =
      ak_hmn_make(sizeof(uint32_t), alct);

    for (uint32_t i = 0; i < 20; i++) {
      uint32_t v = i * 100;
      ak_hmn_insert_u64(&h, i, &v);
    }
    for (uint32_t i = 0; i < 20; i++) {
      uint32_t* v = ak_hmn_at_u64(&h, i);
      ak_assert(v);
      ak_assert(*v == i * 100);
    }

    ak_hmn_destroy(&h);

    ak_log("done.");
  }
  {
    ak_log("basic4");

    ak_hmn h =
      ak_hmn_make(sizeof(uint32_t), alct);

    for (uint32_t i = 0; i < 20; i++) {
      uint32_t v = i * 100;
      ak_hmn_insert_u64(&h, i, &v);
    }
    ak_hmn_remove_u64(&h, 10);

    for (uint32_t i = 0; i < 10; i++) {
      uint32_t* v = ak_hmn_at_u64(&h, i);
      ak_assert(v);
      ak_assert(*v == i * 100);
    }
    {
      ak_assert(!ak_hmn_exist_u64(&h, 10));
    }
    for (uint32_t i = 11; i < 20; i++) {
      uint32_t* v = ak_hmn_at_u64(&h, i);
      ak_assert(v);
      ak_assert(*v == i * 100);
    }

    ak_hmn_destroy(&h);

    ak_log("done.");
  }
  {
    ak_log("basic5");

    ak_hmn h =
      ak_hmn_make(sizeof(uint32_t), alct);

    for (uint32_t i = 0; i < 20; i++) {
      uint32_t v = i * 100;
      ak_hmn_insert_u64(&h, i, &v);
    }

    for (uint32_t i = 0; i < 20; i += 2) {
      uint32_t v = i * 100;
      ak_hmn_remove_u64(&h, i);
    }

    for (uint32_t i = 0; i < 20; i += 2) {
      ak_assert(!ak_hmn_exist_u64(&h, i));
    }
    for (uint32_t i = 1; i < 20; i += 2) {
      uint32_t* v = ak_hmn_at_u64(&h, i);
      ak_assert(v);
      ak_assert(*v == i * 100);
    }

    ak_hmn_destroy(&h);

    ak_log("done.");
  }
  {
    ak_log("basic6");

    ak_hmn h =
      ak_hmn_make(sizeof(uint32_t), alct);

    for (uint32_t i = 0; i < 20; i++) {
      uint32_t v = i * 100;
      ak_hmn_insert_u64(&h, i, &v);
    }

    for (uint32_t i = 0; i < 20; i += 2) {
      uint32_t v = i * 100;
      ak_hmn_remove_u64(&h, i);
    }

    for (uint32_t i = 0; i < 20; i += 2) {
      ak_assert(!ak_hmn_exist_u64(&h, i));
    }
    for (uint32_t i = 1; i < 20; i += 2) {
      uint32_t* v = ak_hmn_at_u64(&h, i);
      ak_assert(v);
      ak_assert(*v == i * 100);
    }

    for (uint32_t i = 20; i < 40; i++) {
      uint32_t v = i * 100;
      ak_hmn_insert_u64(&h, i, &v);
    }

    for (uint32_t i = 20; i < 40; i++) {
      uint32_t* v = ak_hmn_at_u64(&h, i);
      ak_assert(v);
      ak_assert(*v == i * 100);
    }

    ak_hmn_destroy(&h);

    ak_log("done.");
  }
  {
    ak_log("signed");

    ak_hmn h =
      ak_hmn_make(sizeof(uint32_t), alct);

    for (int32_t i = -20; i < 20; i++) {
      uint32_t v = i * 100;
      ak_hmn_insert_i64(&h, i, &v);
    }
    for (int32_t i = -20; i < 20; i++) {
      uint32_t* v = ak_hmn_at_i64(&h, i);
      ak_assert(v);
      ak_assert(*v == i * 100);
    }

    ak_hmn_destroy(&h);

    ak_log("done.");
  }

  ak_log("hmn done");
  ak_program_shutdown();
  return 0;
}
