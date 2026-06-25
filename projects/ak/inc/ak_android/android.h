#ifndef ak_android_android_h
#define ak_android_android_h

#include "ak/export.h"
#include "android_native_app_glue.h"
#include <EGL/egl.h>
#include <stdint.h>

static struct android_app* aks_android_app;

ak_ex void
ak_handle_cmd(struct android_app* app,
              int32_t cmd);

#endif
