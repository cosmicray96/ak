#ifndef ak_export_h
#define ak_export_h

#define ak_ex

#if defined(ak_shared)
#undef ak_ex

#if defined(_WIN32) || defined(__CYGWIN__)
#ifdef ak_inside
#define ak_ex __declspec(dllexport)
#else
#define ak_ex __declspec(dllimport)
#endif

#elif defined(__GNUC__) || defined(__clang__)
#ifdef ak_inside
#define ak_ex                               \
  __attribute__((visibility("default")))
#else
#define ak_ex
#endif

#else
#error "unknown compiler"
#endif

#endif

#endif
