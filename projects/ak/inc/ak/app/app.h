#ifndef ak_app_app_h
#define ak_app_app_h

#include "ak/export.h"

typedef struct ak_app ak_app;

ak_ex ak_app*
ak_app_make();

ak_ex void
ak_app_run();

#endif
