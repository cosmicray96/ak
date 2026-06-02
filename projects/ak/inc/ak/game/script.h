#ifndef ak_game_script_h
#define ak_game_script_h

#include "ak/app/event.h"
#include "ak/game/core.h"
#include "ak/os/time.h"

typedef struct
{
  ak_ett e;
  ak_wv* wv;
  ak_wcb* wcb;
  ak_dur delta;
  ak_evt evt;
} ak_script_ctx;

typedef void (*ak_script_init_fn)(
  ak_script_ctx ctx);
typedef void (*ak_script_deinit_fn)(
  ak_script_ctx ctx);

typedef void (*ak_script_event_fn)(
  ak_script_ctx ctx);
typedef void (*ak_script_update_fn)(
  ak_script_ctx ctx);

typedef struct ak_script ak_script;
struct ak_script
{
  ak_script_init_fn init;
  ak_script_deinit_fn deinit;
  ak_script_event_fn event;
  ak_script_update_fn update;
};

#define ak_as_script_e(name)                \
  ak_script_##name##_e
typedef enum
{
#define ak_script_x(name)                   \
  ak_as_script_e(name),

#include "ak/game/script.inc"
#undef ak_script_x
  ak_script_count_e
} ak_script_enum;

#endif
