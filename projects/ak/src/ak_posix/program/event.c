#define _POSIX_C_SOURCE 200112L

#include "ak/program/event.h"
#include "ak/program/core.h"
#include "ak/program/event_itn.h"

#include <signal.h>
#include <string.h>

#define s_max_signals 64

//===== ak_pmg_event =====//
//--- private ---//
typedef struct
{
  ak_crash_fatal_fn fn;
  struct sigaction sigint_old;
  struct sigaction sigterm_old;
  struct sigaction sigwinch_old;
  struct sigaction sigsegv_old;
} event;
static event e;

//===== signal_queue =====//
//--- private ---//
typedef struct
{
  int signals[s_max_signals];
  volatile sig_atomic_t write_pos;
  volatile sig_atomic_t read_pos;
} signal_queue;
static signal_queue sq = { 0 };

static void
signal_handler(int signum)
{
  sig_atomic_t current_write = sq.write_pos;
  sig_atomic_t next_write =
    (current_write + 1) % s_max_signals;

  // Don't overwrite unread signals
  if (next_write != sq.read_pos) {
    sq.signals[current_write] = signum;
    sq.write_pos = next_write;
  }
}

static void
signal_handler_fatal(int signum)
{
  e.fn();
}

//===== ak_pmg_event =====//
//--- public ---//
void
ak_pgm_event_startup(ak_crash_fatal_fn fn)
{
  e.fn = fn;

  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = signal_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;

  sigaction(SIGINT, &sa, &e.sigint_old);
  sigaction(SIGTERM, &sa, &e.sigterm_old);

  sa.sa_handler = signal_handler_fatal;
  sigaction(SIGSEGV, &sa, &e.sigsegv_old);
}

void
ak_pgm_event_shutdown(void* ctx)
{
  sigaction(SIGINT, &e.sigint_old, NULL);
  sigaction(SIGTERM, &e.sigterm_old, NULL);
  sigaction(SIGSEGV, &e.sigsegv_old, NULL);
}

//--- export ---//
ak_evtpgm
ak_pgm_event_pop()
{
  if (sq.read_pos == sq.write_pos) {
    return ak_evtpgm_none; // empty queue
  }

  int signal = sq.signals[sq.read_pos];
  sq.read_pos =
    (sq.read_pos + 1) % s_max_signals;

  switch (signal) {
    case SIGINT:
    case SIGTERM:
    case SIGWINCH:
      return ak_evtpgm_exit;
      break;
    default:
      return ak_evtpgm_unknown;
      break;
  }
  return ak_evtpgm_unknown;
}

/*
kan_event_os
kan_event_os_get_next()
{
  if (sig_queue.read_pos ==
      sig_queue.write_pos) {
    return kan_event_os_no_event; // Empty
                                  // queue
  }

  int signal =
    sig_queue.signals[sig_queue.read_pos];
  sig_queue.read_pos =
    (sig_queue.read_pos + 1) % s_max_signals;
  switch (signal) {
    case SIGINT:
    case SIGTERM:
      return kan_event_os_kill;
      break;
    default:
      return kan_event_os_unknown;
      break;
  }
}
*/
