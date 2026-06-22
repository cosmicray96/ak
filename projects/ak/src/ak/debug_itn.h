#ifndef ak_debug_itn_h
#define ak_debug_itn_h

void
ak_debug_startup();
void
ak_debug_shutdown(void* ctx);

void
ak_debug_log_impl(const char* log);

void
ak_log_crash_fatal();

#endif
