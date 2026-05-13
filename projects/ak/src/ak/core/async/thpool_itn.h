#ifndef ak_core_async_jobpool_itn_h
#define ak_core_async_jobpool_itn_h

#include "ak/core/async/thpool.h"

ak_thpool*
ak_thpool_startup();
void
ak_thpool_shutdown(ak_thpool* jp);

#endif
