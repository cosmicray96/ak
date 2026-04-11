#ifndef ak_program_event_itn_h
#define ak_program_event_itn_h

typedef void (*ak_crash_fatal_fn)();
void
ak_pgm_event_startup(ak_crash_fatal_fn fn);
void
ak_pgm_event_shutdown();

#endif
