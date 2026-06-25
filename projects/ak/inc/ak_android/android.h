#ifndef ak_android_android_h
#define ak_android_android_h

#include <stdint.h>

static struct android_app* ak_android_app;

static void
ak_handle_cmd(struct android_app* app,
              int32_t cmd);

#endif
