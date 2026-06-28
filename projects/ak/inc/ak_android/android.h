#ifndef ak_android_android_h
#define ak_android_android_h

#include "ak/export.h"
#include "android_native_app_glue.h"
#include <EGL/egl.h>
#include <stdint.h>

ak_ex void
ak_android_app_set(struct android_app* app);
ak_ex struct android_app*
ak_android_app();

#endif
