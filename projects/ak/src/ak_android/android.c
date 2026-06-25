#include "ak_android/android.h"

static struct android_app* aks_android_app =
  0;

static EGLDisplay aks_android_dyp =
  EGL_NO_DISPLAY;
static EGLSurface aks_android_sfc =
  EGL_NO_SURFACE;
static EGLContext aks_android_ctx =
  EGL_NO_CONTEXT;
