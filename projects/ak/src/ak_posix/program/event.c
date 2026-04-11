#define _POSIX_C_SOURCE 200112L

#include "ak/program/event_itn.h"

#include <signal.h>
#include <string.h>

#define s_max_signals 64

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

  /* Don't overwrite unread signals */
  if (next_write != sq.read_pos) {
    sq.signals[current_write] = signum;
    sq.write_pos = next_write;
  }
}

//===== ak_pmg_event =====//
//--- public ---//
void
ak_pgm_event_startup()
{
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = signal_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;

  sigaction(SIGINT, &sa, NULL);
  sigaction(SIGTERM, &sa, NULL);
  sigaction(SIGWINCH, &sa, NULL);
}

void
ak_pgm_event_shutdown()
{
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler =
    SIG_DFL; // Restore default behavior
  sigemptyset(&sa.sa_mask);

  sigaction(SIGINT, &sa, NULL);
  sigaction(SIGTERM, &sa, NULL);
  sigaction(SIGWINCH, &sa, NULL);
}

//--- export ---//
ak_pgm_event
ak_pgm_event_pop()
{
  return 0;
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
