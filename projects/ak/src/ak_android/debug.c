#include "ak/debug_itn.h"
#include "ak_android/android.h"
#include <stdio.h>
#include <string.h>

void
ak_debug_log_impl(const char* log)
{
  return;
  char path[1024];
  snprintf(
    path,
    sizeof(path),
    "%s/app.log",
    aks_android_app->activity->obbPath);

  FILE* f = fopen(path, "a");
  fwrite(log, 1, strlen(log), f);
  fwrite("\n", 1, 1, f);
  fflush(f);
  fclose(f);
}
