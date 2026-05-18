#include "ak/os/cpu.h"

void
ak_cpu_yield()
{
#if defined(__x86_64__) || defined(__i386__)
  _mm_pause();
#elif defined(__aarch64__) ||               \
  defined(__arm__)
  __asm__ volatile("yield");
#elif defined(__powerpc__)
  __asm__ volatile(
    "or 27,27,27"); // PPC "low priority"
                    // hint
#else
  // nothing — bare spin is fine as a
  // fallback
#endif
}
