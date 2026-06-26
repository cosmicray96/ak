#include "ak_android/android.h"

static struct android_app* aks_android_app =
  0;

void
ak_android_app_set(struct android_app* app)
{
  aks_android_app = app;
}

struct android_app*
ak_android_app()
{
  return aks_android_app;
}
