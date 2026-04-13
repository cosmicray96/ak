#ifndef ak_app_app_h
#define ak_app_app_h

#include "ak/app/eq.h"
#include "ak/app/event.h"
#include "ak/coll/dq.h"
#include "ak/core/mem/allocator.h"
#include "ak/os/time.h"
#include <stdbool.h>

typedef struct ak_app ak_app;

//===== ak_applayer =====//
typedef void (*ak_applayer_startup_fn)(
  void* ctx,
  ak_app* app);
typedef void (*ak_applayer_shutdown_fn)(
  void* ctx);
typedef void (*ak_applayer_epusher_fn)(
  void* ctx,
  ak_app_eq* eq);
typedef bool (*ak_applayer_event_fn)(
  void* ctx,
  ak_evt e);
typedef void (*ak_applayer_update_fn)(
  void* ctx,
  ak_dur delta);
typedef void (*ak_applayer_upost_fn)(
  void* ctx,
  ak_dur delta);

typedef struct
{
  ak_applayer_startup_fn on_startup;
  ak_applayer_shutdown_fn on_shutdown;
  ak_applayer_epusher_fn on_epusher;
  ak_applayer_event_fn on_event;
  ak_applayer_update_fn on_update;
  ak_applayer_upost_fn on_upost;
  void* ctx;
} ak_applayer;

//===== ak_applayer_regs =====//
typedef struct
{
  ak_dq layers;
  bool popping;
} ak_applayer_regs;
ak_applayer_regs
ak_applayer_regs_make(ak_alct alct);
void
ak_applayer_regs_destroy(
  ak_applayer_regs* alr);

void
ak_applayer_regs_push(ak_applayer_regs* alr,
                      ak_applayer l);
bool
ak_applayer_regs_pop(ak_applayer_regs* alr,
                     ak_applayer* l);

//===== ak_app  =====//
ak_app*
ak_app_make(ak_applayer_regs* regs,
            ak_alct alct);
void
ak_app_destroy(ak_app* a);

void
ak_app_run(ak_app* a);

void
ak_app_close(ak_app* a);

#endif
